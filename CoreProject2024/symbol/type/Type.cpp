// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Type.hpp"
#include "utils/Macro.hpp"
#include "error/InternalAssert.hpp"
#include "cir/type/CirIntegerType.hpp"
#include "TypeAllocator.hpp"
#include "compiler/CompilerOptions.hpp"

symbol::Type::Type(TypeKind typeKind) noexcept : m_typeKind(typeKind) { }

utils::NoNull<symbol::Type> symbol::Type::make(TypeKind typeKind) {
	error::internalAssert(isPrimitive(typeKind));

	return TypeAllocator::make<Type>(typeKind);
}

utils::NoNull<cir::Type> symbol::Type::makeCirType() const {
	error::internalAssert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::UNIT: return cir::Type::make(cir::TypeKind::UNIT);
		case TypeKind::I8: [[fallthrough]];
		case TypeKind::I16: [[fallthrough]];
		case TypeKind::I32: [[fallthrough]];
		case TypeKind::I64: [[fallthrough]];
		case TypeKind::I128: [[fallthrough]];
		case TypeKind::CINT: [[fallthrough]];
		case TypeKind::CLONG: [[fallthrough]];
		case TypeKind::ISIZE: return cir::IntegerType::make(getTypeSize() * 8, true);
		case TypeKind::U8: [[fallthrough]];
		case TypeKind::U16: [[fallthrough]];
		case TypeKind::U32: [[fallthrough]];
		case TypeKind::U64: [[fallthrough]];
		case TypeKind::U128: [[fallthrough]];
		case TypeKind::USIZE: [[fallthrough]];
		case TypeKind::CUINT: [[fallthrough]];
		case TypeKind::CULONG: return cir::IntegerType::make(getTypeSize() * 8, false);
		case TypeKind::BOOL: return cir::Type::make(cir::TypeKind::BOOL);
		case TypeKind::NEVER_TYPE: return cir::Type::make(cir::TypeKind::UNIT);
	default: unreachable();
	}
}

bool symbol::Type::equals(const Type& other) const noexcept {
	return m_typeKind == other.getTypeKind() 
		|| (isIntegerType() && other.getTypeKind() == TypeKind::INT_LITERAL)
		|| (other.isIntegerType() && m_typeKind == TypeKind::INT_LITERAL);
}

symbol::TypeKind symbol::Type::getTypeKind() const noexcept {
	return m_typeKind;
}

uint32_t symbol::Type::getTypeSize() const noexcept {
	error::internalAssert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::NO_TYPE:		return 0;
		case TypeKind::UNIT:		return 0;
		case TypeKind::I8:			return 1;
		case TypeKind::I16:			return 2;
		case TypeKind::I32:			return 4;
		case TypeKind::I64:			return 8;
		case TypeKind::I128:		return 16;
		case TypeKind::ISIZE:		return compiler::CompilerOptions::getTarget().getPointerSize();
		case TypeKind::U8:			return 1;
		case TypeKind::U16:			return 2;
		case TypeKind::U32:			return 4;
		case TypeKind::U64:			return 8;
		case TypeKind::U128:		return 16;
		case TypeKind::USIZE:		return compiler::CompilerOptions::getTarget().getPointerSize();
		case TypeKind::CINT:		return compiler::CompilerOptions::getTarget().getIntSize();
		case TypeKind::CLONG:		return compiler::CompilerOptions::getTarget().getLongSize();
		case TypeKind::CUINT:		return compiler::CompilerOptions::getTarget().getIntSize();
		case TypeKind::CULONG:		return compiler::CompilerOptions::getTarget().getLongSize();
		case TypeKind::BOOL:		return 1;
		case TypeKind::FUNCTION:	return compiler::CompilerOptions::getTarget().getPointerSize();
		case TypeKind::NEVER_TYPE:	return 0;
	default: unreachable();
	}
}

utf::String symbol::Type::toString() const {
	error::internalAssert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::UNIT:	return "unit";
		case TypeKind::I8:		return "i8";
		case TypeKind::I16:		return "i16";
		case TypeKind::I32:		return "i32";
		case TypeKind::I64:		return "i64";
		case TypeKind::I128:	return "i128";
		case TypeKind::ISIZE:	return "isize";
		case TypeKind::U8:		return "u8";
		case TypeKind::U16:		return "u16";
		case TypeKind::U32:		return "u32";
		case TypeKind::U64:		return "u64";
		case TypeKind::U128:	return "u128";
		case TypeKind::USIZE:	return "usize";
		case TypeKind::CINT:	return "cint";
		case TypeKind::CLONG:	return "clong";
		case TypeKind::CUINT:	return "cuint";
		case TypeKind::CULONG:  return "culong";
		case TypeKind::BOOL:	return "bool";
		case TypeKind::INT_LITERAL:	return "int literal";
	default: unreachable();
	}
}

utf::String symbol::Type::toMangleString() const {
	error::internalAssert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::UNIT:	return "u";
		case TypeKind::I8:		return "i1";
		case TypeKind::I16:		return "i2";
		case TypeKind::I32:		return "i4";
		case TypeKind::I64:		return "i8";
		case TypeKind::I128:	return "i16";
		case TypeKind::ISIZE:	return "is";
		case TypeKind::U8:		return "u1";
		case TypeKind::U16:		return "u2";
		case TypeKind::U32:		return "u4";
		case TypeKind::U64:		return "u8";
		case TypeKind::U128:	return "u16";
		case TypeKind::USIZE:	return "us";
		case TypeKind::CINT:	return "ci";
		case TypeKind::CLONG:	return "cl";
		case TypeKind::CUINT:	return "cui";
		case TypeKind::CULONG:  return "cul";
		case TypeKind::BOOL:	return "b";
	default: unreachable();
	}
}

bool symbol::Type::isNoType() const noexcept {
	return m_typeKind == TypeKind::NO_TYPE;
}

bool symbol::Type::isNeverType() const noexcept {
	return m_typeKind == TypeKind::NEVER_TYPE;
}

bool symbol::Type::isPrimitiveType() const noexcept {
	return isPrimitive(m_typeKind);
}

bool symbol::Type::isComplexType() const noexcept {
	return isComplex(m_typeKind);
}

bool symbol::Type::isLiteralType() const noexcept {
	return isLiteral(m_typeKind);
}

bool symbol::Type::isFunctionType() const noexcept {
	return m_typeKind == TypeKind::FUNCTION;
}

bool symbol::Type::isBoolType() const noexcept {
	return m_typeKind == TypeKind::BOOL;
}

bool symbol::Type::isCCompatibleType() const noexcept {
	return isCCompatible(m_typeKind);
}

bool symbol::Type::isIntegerType() const noexcept {
	return isInteger(m_typeKind);
}

bool symbol::Type::isSignedType() const noexcept {
	return isSigned(m_typeKind);
}

bool symbol::Type::isArithmeticType() const noexcept {
	return isArithmetic(m_typeKind);
}
