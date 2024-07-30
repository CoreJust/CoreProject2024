// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "IdentifierValue.hpp"

ast::IdentifierValue::IdentifierValue(utf::StringView identifier) noexcept 
	: Expression(NodeType::IDENTIFIER_VALUE), m_identifier(identifier) { }

utf::StringView ast::IdentifierValue::getIdentifier() const noexcept {
	return m_identifier;
}
