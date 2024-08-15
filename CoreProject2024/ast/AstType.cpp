// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstType.hpp"

ast::Type::Type() noexcept : m_typeName("") { }

ast::Type::Type(utf::StringView typeName) noexcept : m_typeName(typeName) { }

symbol::Type ast::Type::makeSymbolType() const {
	if (m_typeName == "i32") {
		return symbol::TypeKind::I32;
	} else if (m_typeName == "bool") {
		return symbol::TypeKind::BOOL;
	} else if (m_typeName == "unit") {
		return symbol::TypeKind::UNIT;
	} else {
		return symbol::TypeKind::NO_TYPE;
	}
}

bool ast::Type::isNoType() const noexcept {
	return m_typeName.empty();
}

utf::StringView ast::Type::getTypeName() const noexcept {
	return m_typeName;
}
