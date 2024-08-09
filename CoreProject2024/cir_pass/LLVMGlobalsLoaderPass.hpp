// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	LLVMGlobalsLoaderPass.hpp by CoreJust
*	Created on 06.08.2024
*	Contains LLVMGlobalsLoaderPass class that is derived from ModulePass
*	and that loads all the symbols of CirModule into LLVM module.
*/

#pragma once
#include <unordered_map>
#include "CirPass.hpp"
#include "cir/value/CirValueId.hpp"
#include "llvm_utils/LLVMModule.hpp"

namespace cir {
	class GlobalVariable;
}

namespace cir_pass {
	class LLVMGlobalsLoaderPass final : public ModulePass {
	private:
		llvm_utils::LLVMModule& m_llvmModule;
		std::unordered_map<cir::ValueId, llvm::Value*> m_llvmGlobals;

	public:
		LLVMGlobalsLoaderPass(llvm_utils::LLVMModule& llvmModule) noexcept;

		void pass(utils::NoNull<cir::Module> module) override;

		std::unordered_map<cir::ValueId, llvm::Value*> getLLVMGlobals() noexcept;

	private:
		void loadFunction(utils::NoNull<cir::Function> function);
		void loadGlobalVariable(utils::NoNull<cir::GlobalVariable> variable);
	};
}