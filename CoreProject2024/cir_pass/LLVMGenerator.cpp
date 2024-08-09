// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LLVMGenerator.hpp"
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include "utils/CollectionUtils.hpp"
#include "compiler/CompilerOptions.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/CirValueClassImplementations.hpp"

cir_pass::LLVMGenerator::LLVMGenerator(llvm_utils::LLVMModule& llvmModule, std::unordered_map<cir::ValueId, llvm::Value*> globals) 
	: m_llvmModule(llvmModule), m_symbols(std::move(globals)), m_builder(llvmModule.getContext()) {
	m_targetMachine = compiler::CompilerOptions::getTarget().makeLLVMTargetMachine();
	m_llvmModule.getModule().setDataLayout(m_targetMachine->createDataLayout());

	m_passBuilder = llvm::PassBuilder(m_targetMachine);
	m_passBuilder.registerModuleAnalyses(m_moduleAnalysisManager);
	m_passBuilder.registerCGSCCAnalyses(m_cgsccAnalysisManager);
	m_passBuilder.registerFunctionAnalyses(m_functionAnalysisManager);
	m_passBuilder.registerLoopAnalyses(m_loopAnalysisManager);
	m_passBuilder.crossRegisterProxies(
		m_loopAnalysisManager,
		m_functionAnalysisManager,
		m_cgsccAnalysisManager,
		m_moduleAnalysisManager
	);

	llvm::OptimizationLevel optLevel = llvm::OptimizationLevel::O2;
	switch (compiler::CompilerOptions::getOptimizationLevel()) {
		case compiler::OptimizationLevel::O0: optLevel = llvm::OptimizationLevel::O0;
		case compiler::OptimizationLevel::O1: optLevel = llvm::OptimizationLevel::O1;
		case compiler::OptimizationLevel::O2: optLevel = llvm::OptimizationLevel::O2;
		case compiler::OptimizationLevel::O3: optLevel = llvm::OptimizationLevel::O3;
		case compiler::OptimizationLevel::OSIZE: optLevel = llvm::OptimizationLevel::Oz;
	default: break;
	}

	m_modulePassManager = m_passBuilder.buildPerModuleDefaultPipeline(optLevel);
}

void cir_pass::LLVMGenerator::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isFunction()) {
			compileFunction(value.as<cir::Function>());
		}
	}
}

void cir_pass::LLVMGenerator::optimize() {
	m_modulePassManager.run(m_llvmModule.getModule(), m_moduleAnalysisManager);
}

llvm::TargetMachine* cir_pass::LLVMGenerator::getTargetMachine() noexcept {
	return m_targetMachine;
}

void cir_pass::LLVMGenerator::compileFunction(utils::NoNull<cir::Function> function) {
	if (!function->isNativeFunction()) {
		llvm::Function* llvmFunction = reinterpret_cast<llvm::Function*>(m_symbols[function->getId()]);
		llvm::BasicBlock* llvmBB = llvm::BasicBlock::Create(m_llvmModule.getContext(), "entry", llvmFunction);
		m_builder.SetInsertPoint(llvmBB);

		m_values.clear();

		// Adding function arguments to variable list.
		std::vector<utils::NoNull<cir::FunctionArgument>> functionArguments = function->getArguments();
		for (uint32_t i = 0; i < functionArguments.size(); i++) {
			llvm::Value* llvmArgument = m_builder.CreateAlloca(llvmFunction->getArg(i)->getType(), 0, "arg$" + utf::String(functionArguments[i]->getName()));
			m_builder.CreateStore(llvmFunction->getArg(i), llvmArgument);
			m_values.try_emplace(functionArguments[i]->getId(), llvmArgument);
		}

		for (utils::NoNull<cir::BasicBlock> basicBlock : function.as<cir::CommonFunction>()->getBasicBlocks()) {
			compileBasicBlock(basicBlock, llvmFunction);
		}

		llvm::verifyFunction(*llvmFunction);
		m_functionPassManager.run(*llvmFunction, m_functionAnalysisManager);
	}
}

void cir_pass::LLVMGenerator::compileBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock, llvm::Function* llvmFunction) {
	if (llvmFunction->back().getTerminator() != nullptr) {
		llvm::BasicBlock* llvmBB = llvm::BasicBlock::Create(m_llvmModule.getContext(), basicBlock->getName(), llvmFunction);
		m_builder.SetInsertPoint(llvmBB);
	}

	for (utils::NoNull<cir::Instruction> instruction : basicBlock->getInstructions()) {
		m_values.try_emplace(instruction->getId(), compileInstruction(instruction));
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileInstruction(utils::NoNull<cir::Instruction> instruction) {
	switch (instruction->getKind()) {
		case cir::ValueKind::UNARY_INSTRUCTION: return compileUnaryInstruction(instruction.as<cir::UnaryInstruction>());
		case cir::ValueKind::BINARY_INSTRUCTION: return compileBinaryInstruction(instruction.as<cir::BinaryInstruction>());
		case cir::ValueKind::INVOCATION_INSTRUCTION: return compileInvocationInstruction(instruction.as<cir::InvocationInstruction>());
		case cir::ValueKind::LOCAL_VARIABLE: return compileLocalVariable(instruction.as<cir::LocalVariable>());
		case cir::ValueKind::UNIT_INVOCATION_INSTRUCTION: return compileUnitInvocationInstruction(instruction.as<cir::UnitInvocationInstruction>());
		case cir::ValueKind::RET_INSTRUCTION: return compileRetInstruction(instruction.as<cir::RetInstruction>());
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::getLLVMValue(utils::NoNull<cir::Value> value) {
	if (value->isInstruction() || value->getKind() == cir::ValueKind::FUNCTION_ARGUMENT) {
		llvm::Value* result = m_values[value->getId()];
		if (result->getType()->isPointerTy()) { // Temporary for handling variables.
			result = m_builder.CreateLoad(value->getType().makeLLVMType(m_llvmModule.getContext()), result);
		}

		return result;
	} else {
		return compileConstant(value.as<cir::Constant>());
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileConstant(utils::NoNull<cir::Constant> constant) {
	switch (constant->getKind()) {
		case cir::ValueKind::CONSTANT_NUMBER: return llvm::ConstantInt::get(m_llvmModule.getContext(), llvm::APInt(32, constant.as<cir::ConstantNumber>()->getValue(), true));
		case cir::ValueKind::GLOBAL_VARIABLE: return m_builder.CreateLoad(constant->getType().makeLLVMType(m_llvmModule.getContext()), m_symbols[constant->getId()]);
		case cir::ValueKind::COMMON_FUNCTION: [[fallthrough]];
		case cir::ValueKind::NATIVE_FUNCTION: return m_symbols[constant->getId()];
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileUnaryInstruction(utils::NoNull<cir::UnaryInstruction> instruction) {
	llvm::Value* operand = getLLVMValue(instruction->getOperand());
	switch (instruction->getInstructionKind()) {
		case cir::UnaryInstruction::NEG: return m_builder.CreateNeg(operand, instruction->getName());
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileBinaryInstruction(utils::NoNull<cir::BinaryInstruction> instruction) {
	llvm::Value* left = getLLVMValue(instruction->getLeft());
	llvm::Value* right = getLLVMValue(instruction->getRight());
	switch (instruction->getInstructionKind()) {
		case cir::BinaryInstruction::ADD: return m_builder.CreateAdd(left, right, instruction->getName());
		case cir::BinaryInstruction::SUB: return m_builder.CreateSub(left, right, instruction->getName());
		case cir::BinaryInstruction::MUL: return m_builder.CreateMul(left, right, instruction->getName());
		case cir::BinaryInstruction::DIV: return m_builder.CreateSDiv(left, right, instruction->getName());
		case cir::BinaryInstruction::REM: return m_builder.CreateSRem(left, right, instruction->getName());
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileInvocationInstruction(utils::NoNull<cir::InvocationInstruction> instruction) {
	llvm::Function* callee = reinterpret_cast<llvm::Function*>(getLLVMValue(instruction->getCallee()));
	std::vector<llvm::Value*> arguments = utils::map<std::vector<llvm::Value*>>(
		instruction->getArguments(),
		[this](auto argument) -> auto {
			return getLLVMValue(argument);
		}
	);

	return m_builder.CreateCall(callee->getFunctionType(), callee, arguments, instruction->getName());
}

llvm::Value* cir_pass::LLVMGenerator::compileLocalVariable(utils::NoNull<cir::LocalVariable> instruction) {
	llvm::Value* initialValue = getLLVMValue(instruction->getInitialValue());
	llvm::Value* result = m_builder.CreateAlloca(instruction->getType().makeLLVMType(m_llvmModule.getContext()), 0, instruction->getName());
	m_builder.CreateStore(initialValue, result);
	return result;
}

llvm::Value* cir_pass::LLVMGenerator::compileUnitInvocationInstruction(utils::NoNull<cir::UnitInvocationInstruction> instruction) {
	llvm::Function* callee = reinterpret_cast<llvm::Function*>(getLLVMValue(instruction->getCallee()));
	std::vector<llvm::Value*> arguments = utils::map<std::vector<llvm::Value*>>(
		instruction->getArguments(),
		[this](auto argument) -> auto {
			return getLLVMValue(argument);
		}
	);

	return m_builder.CreateCall(callee->getFunctionType(), callee, arguments);
}

llvm::Value* cir_pass::LLVMGenerator::compileRetInstruction(utils::NoNull<cir::RetInstruction> instruction) {
	if (instruction->isRetUnit()) {
		return m_builder.CreateRetVoid();
	} else {
		llvm::Value* value = getLLVMValue(instruction->getOperand());
		return m_builder.CreateRet(value);
	}
}
