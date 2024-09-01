// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirFunctionType.hpp"
#include <format>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include "utils/CollectionUtils.hpp"
#include "compiler/CompilerOptions.hpp"
#include "CirTypeAllocator.hpp"

cir::FunctionType::FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept 
    : Type(TypeKind::FUNCTION), m_returnType(returnType), m_argumentTypes(std::move(argumentTypes)) { }

utils::NoNull<cir::FunctionType> cir::FunctionType::make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) {
    return CirTypeAllocator::make<FunctionType>(returnType, std::move(argumentTypes));
}

llvm::FunctionType* cir::FunctionType::makeRawLLVMFunctionType(llvm::LLVMContext& context) const {
    return llvm::FunctionType::get(
        m_returnType->makeLLVMType(context),
        utils::map<std::vector<llvm::Type*>>(
            m_argumentTypes,
            [&context](auto argumentType) -> auto {
                return argumentType->makeLLVMType(context);
            }
        ),
        false
    );
}

llvm::Type* cir::FunctionType::makeLLVMType(llvm::LLVMContext& context) const {
    return llvm::PointerType::get(makeRawLLVMFunctionType(context), 0);
}

bool cir::FunctionType::equals(utils::NoNull<Type> other) const noexcept {
    if (other->getTypeKind() != TypeKind::FUNCTION) {
        return false;
    }

    utils::NoNull<FunctionType> otherAsFunction = other.as<FunctionType>();
    if (!otherAsFunction->getReturnType()->equals(m_returnType)) {
        return false;
    }

    const std::vector<utils::NoNull<Type>>& otherArguments = otherAsFunction->getArgumentTypes();
    if (otherArguments.size() != m_argumentTypes.size()) {
        return false;
    }

    for (size_t i = 0; i < m_argumentTypes.size(); ++i) {
        if (!m_argumentTypes[i]->equals(otherArguments[i])) {
            return false;
        }
    }

    return true;
}

uint32_t cir::FunctionType::getTypeSize() const noexcept {
    return compiler::CompilerOptions::getTarget().getPointerSize();
}

const std::vector<utils::NoNull<cir::Type>>& cir::FunctionType::getArgumentTypes() const noexcept {
    return m_argumentTypes;
}

utils::NoNull<cir::Type> cir::FunctionType::getReturnType() const noexcept {
    return m_returnType;
}

utf::String cir::FunctionType::toString() const {
    return std::format("fn({}): {}", utils::joinToString(m_argumentTypes), m_returnType->toString());
}
