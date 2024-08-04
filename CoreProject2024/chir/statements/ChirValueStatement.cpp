// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirValueStatement.hpp"

chir::ValueStatement::ValueStatement(utils::NoNull<Value> value) noexcept
	: Statement(NodeType::VALUE_STATEMENT), m_value(value) {
	Node::setParent(value, this);
}

utils::NoNull<chir::Value>& chir::ValueStatement::getValue() noexcept {
	return m_value;
}