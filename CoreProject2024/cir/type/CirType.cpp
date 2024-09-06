// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirType.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include "utils/Macro.hpp"
#include "CirTypeAllocator.hpp"

cir::Type::Type(TypeKind kind) noexcept : m_typeKind(kind) { }

utils::NoNull<cir::Type> cir::Type::make(TypeKind typeKind) {
	internalAssert(isPrimitive(typeKind));

	return CirTypeAllocator::make<Type>(typeKind);
}

llvm::Type* cir::Type::makeLLVMType(llvm::LLVMContext& context) const {
	switch (m_typeKind) {
		case TypeKind::UNIT: return llvm::Type::getVoidTy(context);
		case TypeKind::BOOL: return llvm::Type::getInt1Ty(context);
		case TypeKind::BASIC_BLOCK: return llvm::Type::getLabelTy(context);
	default: unreachable();
	}
}

bool cir::Type::equals(utils::NoNull<Type> other) const noexcept {
	return m_typeKind == other->getTypeKind();
}

cir::TypeKind cir::Type::getTypeKind() const noexcept {
	return m_typeKind;
}

uint32_t cir::Type::getTypeSize() const noexcept {
	switch (m_typeKind) {
		case TypeKind::UNIT: return 0;
		case TypeKind::BOOL: return 1;
		case TypeKind::BASIC_BLOCK: return 8;
	default: unreachable();
	}
}

utf::String cir::Type::toString() const {
	switch (m_typeKind) {
		case TypeKind::UNIT: return "unit";
		case TypeKind::BOOL: return "bool";
		case TypeKind::BASIC_BLOCK: return "bb";
	default: unreachable();
	}
}

