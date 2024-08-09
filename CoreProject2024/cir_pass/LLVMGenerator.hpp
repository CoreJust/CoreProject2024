// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	LLVMGenerator.hpp by CoreJust
*	Created on 06.08.2024
*	Contains LLVMGenerator class that is derived from ModulePass
*	and that compiles CIR Module into LLVM Module.
*/

#pragma once
#include <unordered_map>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include "CirPass.hpp"
#include "cir/value/CirValueId.hpp"
#include "cir/value/CirValueClassDeclarations.hpp"
#include "llvm_utils/LLVMModule.hpp"

namespace cir {
	class Constant;
}

namespace cir_pass {
	class LLVMGenerator final : public ModulePass {
	private:
		llvm_utils::LLVMModule& m_llvmModule;
		llvm::IRBuilder<> m_builder;
		std::unordered_map<cir::ValueId, llvm::Value*> m_symbols;
		
		// Reset for each function, contains the llvm::Value for each Instruction.
		std::unordered_map<cir::ValueId, llvm::Value*> m_values;

		llvm::TargetMachine* m_targetMachine;

		llvm::PassBuilder m_passBuilder;
		llvm::FunctionPassManager m_functionPassManager;
		llvm::ModulePassManager m_modulePassManager;

		llvm::LoopAnalysisManager m_loopAnalysisManager;
		llvm::FunctionAnalysisManager m_functionAnalysisManager;
		llvm::CGSCCAnalysisManager m_cgsccAnalysisManager;
		llvm::ModuleAnalysisManager m_moduleAnalysisManager;

	public:
		LLVMGenerator(llvm_utils::LLVMModule& llvmModule, std::unordered_map<cir::ValueId, llvm::Value*> globals);

		void pass(utils::NoNull<cir::Module> module) override;

		llvm::TargetMachine* getTargetMachine() noexcept;

	private:
		void compileFunction(utils::NoNull<cir::Function> function);
		void compileBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock, llvm::Function* llvmFunction);
		llvm::Value* compileInstruction(utils::NoNull<cir::Instruction> instruction);
		llvm::Value* getLLVMValue(utils::NoNull<cir::Value> value);
		llvm::Value* compileConstant(utils::NoNull<cir::Constant> constant);

		llvm::Value* compileUnaryInstruction(utils::NoNull<cir::UnaryInstruction> instruction);
		llvm::Value* compileBinaryInstruction(utils::NoNull<cir::BinaryInstruction> instruction);
		llvm::Value* compileInvocationInstruction(utils::NoNull<cir::InvocationInstruction> instruction);
		llvm::Value* compileLocalVariable(utils::NoNull<cir::LocalVariable> instruction);
		llvm::Value* compileUnitInvocationInstruction(utils::NoNull<cir::UnitInvocationInstruction> instruction);
		llvm::Value* compileRetInstruction(utils::NoNull<cir::RetInstruction> instruction);
	};
}