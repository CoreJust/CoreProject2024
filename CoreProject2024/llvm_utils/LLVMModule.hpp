// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	LLVMModule.hpp by CoreJust
*	Created on 06.08.2024
*	Contains the LLVMModule class that contains llvm::Module and LLVMContext, as well as some additional
*	info for interoperating with LLVM IR.
*/

#pragma once
#include "utils/NoNull.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace llvm_utils {
	class LLVMModule final {
	private:
		utils::NoNull<llvm::LLVMContext> m_context;
		llvm::Module m_module;

	public:
		LLVMModule(utils::NoNull<llvm::LLVMContext> context, utf::String name);

		llvm::LLVMContext& getContext() noexcept;
		llvm::Module& getModule() noexcept;
	};
}
