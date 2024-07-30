// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LiteralValue.hpp"

ast::LiteralValue::LiteralValue(utf::StringView value) noexcept 
	: Expression(NodeType::LITERAL_VALUE), m_value(value) { }

utf::StringView ast::LiteralValue::getValue() const noexcept {
	return m_value;
}
