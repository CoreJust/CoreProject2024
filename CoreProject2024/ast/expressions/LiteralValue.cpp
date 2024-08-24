// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LiteralValue.hpp"
#include "error/InternalAssert.hpp"

// Values used for bool literals.
const utf::StringView BOOL_FALSE = "\0";
const utf::StringView BOOL_TRUE = "\x1";

ast::LiteralValue::LiteralValue(utf::StringView value) noexcept 
	: Expression(NodeKind::LITERAL_VALUE), m_value(value), m_type(Type::make(TypeKind::INT_LITERAL)) { }

ast::LiteralValue::LiteralValue(bool value) noexcept
	: Expression(NodeKind::LITERAL_VALUE), m_value(value ? BOOL_TRUE : BOOL_FALSE), m_type(Type::make(TypeKind::BOOL)) { }

utils::NoNull<ast::Type> ast::LiteralValue::getType() const noexcept {
	return m_type;
}

utf::StringView ast::LiteralValue::getRawValue() const noexcept {
	return m_value;
}

utils::IntValue ast::LiteralValue::parseAsInt() const noexcept {
	error::internalAssert(m_type->getKind() == TypeKind::INT_LITERAL, "Must be an integer");

	return utils::IntValue(utf::String(m_value));
}

bool ast::LiteralValue::parseAsBool() const noexcept {
	error::internalAssert(m_type->getKind() == TypeKind::BOOL, "Must be bool");

	return m_value[0];
}
