// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LLVMModule.hpp"

llvm_utils::LLVMModule::LLVMModule(utf::String name) 
    : m_context(), m_module(name, m_context) { }

llvm::LLVMContext& llvm_utils::LLVMModule::getContext() noexcept {
    return m_context;
}

llvm::Module& llvm_utils::LLVMModule::getModule() noexcept {
    return m_module;
}
