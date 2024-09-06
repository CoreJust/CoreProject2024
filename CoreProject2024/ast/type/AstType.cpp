// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstType.hpp"
#include "utils/Macro.hpp"
#include "error/InternalAssert.hpp"
#include "symbol/type/Type.hpp"
#include "AstTypeAllocator.hpp"

ast::Type::Type(TypeKind typeKind) noexcept : m_typeKind(typeKind) { }

ast::Type::Type() noexcept : m_typeKind(TypeKind::NO_TYPE) { }

utils::NoNull<ast::Type> ast::Type::make(TypeKind typeKind) {
	internalAssert(isPrimitive(typeKind));

	return AstTypeAllocator::make<Type>(typeKind);
}

utils::NoNull<symbol::Type> ast::Type::makeSymbolType() const {
	switch (m_typeKind) {
		case TypeKind::NO_TYPE:	return symbol::Type::make(symbol::TypeKind::NO_TYPE);
		case TypeKind::UNIT:	return symbol::Type::make(symbol::TypeKind::UNIT);
		case TypeKind::I8:		return symbol::Type::make(symbol::TypeKind::I8);
		case TypeKind::I16:		return symbol::Type::make(symbol::TypeKind::I16);
		case TypeKind::I32:		return symbol::Type::make(symbol::TypeKind::I32);
		case TypeKind::I64:		return symbol::Type::make(symbol::TypeKind::I64);
		case TypeKind::I128:	return symbol::Type::make(symbol::TypeKind::I128);
		case TypeKind::ISIZE:	return symbol::Type::make(symbol::TypeKind::ISIZE);
		case TypeKind::U8:		return symbol::Type::make(symbol::TypeKind::U8);
		case TypeKind::U16:		return symbol::Type::make(symbol::TypeKind::U16);
		case TypeKind::U32:		return symbol::Type::make(symbol::TypeKind::U32);
		case TypeKind::U64:		return symbol::Type::make(symbol::TypeKind::U64);
		case TypeKind::U128:	return symbol::Type::make(symbol::TypeKind::U128);
		case TypeKind::USIZE:	return symbol::Type::make(symbol::TypeKind::USIZE);
		case TypeKind::CINT:	return symbol::Type::make(symbol::TypeKind::CINT);
		case TypeKind::CLONG:	return symbol::Type::make(symbol::TypeKind::CLONG);
		case TypeKind::CUINT:	return symbol::Type::make(symbol::TypeKind::CUINT);
		case TypeKind::CULONG:  return symbol::Type::make(symbol::TypeKind::CULONG);
		case TypeKind::BOOL:	return symbol::Type::make(symbol::TypeKind::BOOL);
		case TypeKind::INT_LITERAL:	return symbol::Type::make(symbol::TypeKind::INT_LITERAL);
	default: unreachable();
	}
}

bool ast::Type::isNoType() const noexcept {
	return m_typeKind == TypeKind::NO_TYPE;
}

bool ast::Type::isPrimitiveType() const noexcept {
	return isPrimitive(m_typeKind);
}

bool ast::Type::isComplexType() const noexcept {
	return isComplex(m_typeKind);
}

bool ast::Type::isFunctionType() const noexcept {
	return m_typeKind == TypeKind::FUNCTION;
}

bool ast::Type::isCCompatibleType() const noexcept {
	return isCCompatible(m_typeKind);
}

bool ast::Type::isIntegerType() const noexcept {
	return isInteger(m_typeKind);
}

ast::TypeKind ast::Type::getKind() const noexcept {
	return m_typeKind;
}

utf::String ast::Type::toString() const noexcept {
	switch (m_typeKind) {
		case TypeKind::NO_TYPE: return "";
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
