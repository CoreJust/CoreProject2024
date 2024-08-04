// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirVariableDeclaration.hpp"

chir::VariableDeclaration::VariableDeclaration(const symbol::VariableSymbol& variable, utils::NoNull<Value> initialValue) noexcept
    : Declaration(NodeType::VARIABLE_DECLARATION), m_variable(variable), m_initialValue(initialValue) {
    Node::setParent(m_initialValue, this);
}

const symbol::VariableSymbol& chir::VariableDeclaration::getVariable() noexcept {
    return m_variable;
}

utils::NoNull<chir::Value>& chir::VariableDeclaration::getInitialValue() noexcept {
    return m_initialValue;
}
