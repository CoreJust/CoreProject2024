// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirVariableStatement.hpp"
#include "symbol/SymbolAllocator.hpp"

chir::VariableStatement::VariableStatement(symbol::VariableSymbol& variable, utils::NoNull<Value> initialValue) noexcept
	: Statement(NodeKind::VARIABLE_STATEMENT), m_variable(variable), m_initialValue(initialValue) {
	Node::setParent(m_initialValue, this);
}

symbol::VariableSymbol& chir::VariableStatement::getVariable() noexcept {
	return m_variable;
}

utils::NoNull<chir::Value>& chir::VariableStatement::getInitialValue() noexcept {
	return m_initialValue;
}
