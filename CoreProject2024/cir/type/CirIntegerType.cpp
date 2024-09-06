// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirIntegerType.hpp"
#include <format>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include "CirTypeAllocator.hpp"

cir::IntegerType::IntegerType(uint32_t bitSize, bool isSigned) noexcept 
	: Type(TypeKind::INTEGER), m_bitSize(bitSize), m_isSigned(isSigned) { }

utils::NoNull<cir::IntegerType> cir::IntegerType::make(uint32_t bitSize, bool isSigned) {
	internalAssert(bitSize != 0);

	return CirTypeAllocator::make<IntegerType>(bitSize, isSigned);
}

llvm::Type* cir::IntegerType::makeLLVMType(llvm::LLVMContext& context) const {
	return llvm::Type::getIntNTy(context, m_bitSize);
}

bool cir::IntegerType::equals(utils::NoNull<Type> other) const noexcept {
	if (other->getTypeKind() != TypeKind::INTEGER) {
		return false;
	}

	utils::NoNull<IntegerType> otherAsInteger = other.as<IntegerType>();
	return otherAsInteger->getBitSize() == m_bitSize && otherAsInteger->isSigned() == m_isSigned;
}

uint32_t cir::IntegerType::getTypeSize() const noexcept {
	return m_bitSize >> 3;
}

uint32_t cir::IntegerType::getBitSize() const noexcept {
	return m_bitSize;
}

bool cir::IntegerType::isSigned() const noexcept {
	return m_isSigned;
}

utf::String cir::IntegerType::toString() const {
	return std::format("{}{}", char('u' - m_isSigned * ('u' - 'i')), m_bitSize);
}
