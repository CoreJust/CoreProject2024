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
#include "utils/Macro.hpp"
#include "compiler/CompilerOptions.hpp"
#include "cir/CirModule.hpp"
#include "cir/type/CirIntegerType.hpp"
#include "cir/type/CirFunctionType.hpp"
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
		case compiler::OptimizationLevel::O0: optLevel = llvm::OptimizationLevel::O0; break;
		case compiler::OptimizationLevel::O1: optLevel = llvm::OptimizationLevel::O1; break;
		case compiler::OptimizationLevel::O2: optLevel = llvm::OptimizationLevel::O2; break;
		case compiler::OptimizationLevel::O3: optLevel = llvm::OptimizationLevel::O3; break;
		case compiler::OptimizationLevel::OSIZE: optLevel = llvm::OptimizationLevel::Oz; break;
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
		utils::NoNull<cir::CommonFunction> asCommonFunction = function.as<cir::CommonFunction>();

		m_values.clear();

		// Create an LLVM basic block for each of the basic blocks of the function.
		for (utils::NoNull<cir::BasicBlock> basicBlock : asCommonFunction->getBasicBlocks()) {
			m_values.try_emplace(basicBlock->getId(), llvm::BasicBlock::Create(m_llvmModule.getContext(), basicBlock->getName(), llvmFunction));
		}

		m_builder.SetInsertPoint(getLLVMBB(asCommonFunction->getBasicBlocks().front()));

		// Adding function arguments to variable list.
		std::vector<utils::NoNull<cir::FunctionArgument>>& functionArguments = function->getArguments();
		for (uint32_t i = 0; i < functionArguments.size(); i++) {
			llvm::Value* llvmArgument = m_builder.CreateAlloca(llvmFunction->getArg(i)->getType(), 0, "arg$" + utf::String(functionArguments[i]->getName()));
			m_builder.CreateStore(llvmFunction->getArg(i), llvmArgument);
			m_values.try_emplace(functionArguments[i]->getId(), llvmArgument);
		}

		for (utils::NoNull<cir::BasicBlock> basicBlock : asCommonFunction->getBasicBlocks()) {
			compileBasicBlock(basicBlock, llvmFunction);
		}

		llvm::verifyFunction(*llvmFunction);
		m_functionPassManager.run(*llvmFunction, m_functionAnalysisManager);
	}
}

void cir_pass::LLVMGenerator::compileBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock, llvm::Function* llvmFunction) {
	m_builder.SetInsertPoint(getLLVMBB(basicBlock));

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
		case cir::ValueKind::GOTO_INSTRUCTION: return compileGotoInstruction(instruction.as<cir::GotoInstruction>());
		case cir::ValueKind::BRANCH_INSTRUCTION: return compileBranchInstruction(instruction.as<cir::BranchInstruction>());
		case cir::ValueKind::RET_INSTRUCTION: return compileRetInstruction(instruction.as<cir::RetInstruction>());
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::getLLVMValue(utils::NoNull<cir::Value> value) {
	if (value->isInstruction() || value->getKind() == cir::ValueKind::FUNCTION_ARGUMENT) {
		llvm::Value* result = m_values[value->getId()];
		if (value->isGlobalVariable() || value->getKind() == cir::ValueKind::LOCAL_VARIABLE || value->getKind() == cir::ValueKind::FUNCTION_ARGUMENT) { // Temporary for handling variables.
			result = m_builder.CreateLoad(value->getType()->makeLLVMType(m_llvmModule.getContext()), result);
		}

		return result;
	} else {
		return compileConstant(value.as<cir::Constant>());
	}
}

llvm::BasicBlock* cir_pass::LLVMGenerator::getLLVMBB(utils::NoNull<cir::BasicBlock> basicBlock) {
	return reinterpret_cast<llvm::BasicBlock*>(m_values[basicBlock->getId()]);
}

llvm::Value* cir_pass::LLVMGenerator::compileConstant(utils::NoNull<cir::Constant> constant) {
	switch (constant->getKind()) {
		case cir::ValueKind::CONSTANT_NUMBER: return llvm::ConstantInt::get(
			m_llvmModule.getContext(), 
			llvm::APInt(
				constant->getType()->getTypeSize() * 8,
				constant.as<cir::ConstantNumber>()->getValue().str(10), 
				10
			)
		);
		case cir::ValueKind::GLOBAL_VARIABLE: return m_builder.CreateLoad(constant->getType()->makeLLVMType(m_llvmModule.getContext()), m_symbols[constant->getId()]);
		case cir::ValueKind::COMMON_FUNCTION: [[fallthrough]];
		case cir::ValueKind::NATIVE_FUNCTION: return m_symbols[constant->getId()];
		case cir::ValueKind::BASIC_BLOCK: return m_values[constant->getId()];
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileUnaryInstruction(utils::NoNull<cir::UnaryInstruction> instruction) {
	llvm::Value* operand = getLLVMValue(instruction->getOperand());
	switch (instruction->getInstructionKind()) {
		case cir::UnaryInstruction::NEG:	   return m_builder.CreateNeg(operand, instruction->getName());
		case cir::UnaryInstruction::LOGIC_NOT: return m_builder.CreateNot(operand, instruction->getName());
		case cir::UnaryInstruction::CAST:	   return compileValueCast(operand, instruction->getOperand()->getType(), instruction->getType(), instruction->getName());
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
		case cir::BinaryInstruction::LOGIC_AND: return m_builder.CreateAnd(left, right, instruction->getName());
		case cir::BinaryInstruction::LOGIC_OR: return m_builder.CreateOr(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_EQ: return m_builder.CreateICmpEQ(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_NEQ: return m_builder.CreateICmpNE(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_LEQ: return m_builder.CreateICmpSLE(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_GEQ: return m_builder.CreateICmpSGE(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_LT: return m_builder.CreateICmpSLT(left, right, instruction->getName());
		case cir::BinaryInstruction::CMP_GT: return m_builder.CreateICmpSGT(left, right, instruction->getName());
	default: unreachable();
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileInvocationInstruction(utils::NoNull<cir::InvocationInstruction> instruction) {
	llvm::Value* callee = getFunctionValue(instruction->getCallee());
	std::vector<llvm::Value*> arguments = utils::map<std::vector<llvm::Value*>>(
		instruction->getArguments(),
		[this](auto argument) -> auto {
			return getLLVMValue(argument);
		}
	);

	return m_builder.CreateCall(instruction->getCallee()->getType().as<cir::FunctionType>()->makeRawLLVMFunctionType(m_llvmModule.getContext()), callee, arguments, instruction->getName());
}

llvm::Value* cir_pass::LLVMGenerator::compileLocalVariable(utils::NoNull<cir::LocalVariable> instruction) {
	llvm::Value* initialValue = getLLVMValue(instruction->getInitialValue());
	llvm::Value* result = m_builder.CreateAlloca(instruction->getType()->makeLLVMType(m_llvmModule.getContext()), 0, instruction->getName());
	m_builder.CreateStore(initialValue, result);
	return result;
}

llvm::Value* cir_pass::LLVMGenerator::compileUnitInvocationInstruction(utils::NoNull<cir::UnitInvocationInstruction> instruction) {
	llvm::Value* callee = getFunctionValue(instruction->getCallee());
	std::vector<llvm::Value*> arguments = utils::map<std::vector<llvm::Value*>>(
		instruction->getArguments(),
		[this](auto argument) -> auto {
			return getLLVMValue(argument);
		}
	);

	return m_builder.CreateCall(instruction->getCallee()->getType().as<cir::FunctionType>()->makeRawLLVMFunctionType(m_llvmModule.getContext()), callee, arguments);
}

llvm::Value* cir_pass::LLVMGenerator::compileGotoInstruction(utils::NoNull<cir::GotoInstruction> instruction) {
	return m_builder.CreateBr(getLLVMBB(instruction->getBasicBlockToGo()));
}

llvm::Value* cir_pass::LLVMGenerator::compileBranchInstruction(utils::NoNull<cir::BranchInstruction> instruction) {
	return m_builder.CreateCondBr(getLLVMValue(instruction->getCondition()), getLLVMBB(instruction->getSuccessBranch()), getLLVMBB(instruction->getFailureBranch()));
}

llvm::Value* cir_pass::LLVMGenerator::compileRetInstruction(utils::NoNull<cir::RetInstruction> instruction) {
	if (instruction->isRetUnit()) {
		return m_builder.CreateRetVoid();
	} else {
		llvm::Value* value = getLLVMValue(instruction->getOperand());
		return m_builder.CreateRet(value);
	}
}

llvm::Value* cir_pass::LLVMGenerator::compileValueCast(llvm::Value* operand, utils::NoNull<cir::Type> originalType, utils::NoNull<cir::Type> resultType, utf::StringView name) {
	if (originalType->equals(resultType)) {
		return operand;
	}

	llvm::Type* resultLLVMType = resultType->makeLLVMType(m_llvmModule.getContext());
	if (originalType->getTypeKind() == cir::TypeKind::INTEGER || originalType->getTypeKind() == cir::TypeKind::BOOL) {
		if (resultType->getTypeKind() == cir::TypeKind::INTEGER) { // Integer to integer cast
			if (originalType->getTypeSize() == resultType->getTypeSize()) {
				return operand; // Signedness cast doesn't need to be performed in LLVM IR.
			}

			return m_builder.CreateIntCast(operand, resultLLVMType, originalType.as<cir::IntegerType>()->isSigned(), name);
		} else if (resultType->getTypeKind() == cir::TypeKind::FUNCTION) {
			return m_builder.CreateIntToPtr(operand, resultLLVMType, name);
		}
	} else if (originalType->getTypeKind() == cir::TypeKind::FUNCTION) {
		if (resultType->getTypeKind() == cir::TypeKind::INTEGER) {
			return m_builder.CreatePtrToInt(operand, resultLLVMType, name);
		} else if (resultType->getTypeKind() == cir::TypeKind::FUNCTION) {
			return m_builder.CreateBitCast(operand, resultLLVMType, name);
		}
	}

	unreachable();
}

llvm::Value* cir_pass::LLVMGenerator::getFunctionValue(utils::NoNull<cir::Value> value) {
	error::internalAssert(value->getType()->getTypeKind() == cir::TypeKind::FUNCTION);

	llvm::Value* source = getLLVMValue(value);
	return source;
}
