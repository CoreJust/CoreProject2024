// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "VariableDeclaration.hpp"

ast::VariableDeclaration::VariableDeclaration(utf::StringView name, ast::Type type, utils::NoNull<Expression> initialValue) noexcept
    : Declaration(NodeType::VARIABLE_DECLARATION), m_name(name), m_type(type), m_initialValue(initialValue) {
    Node::setParent(m_initialValue.get(), this);
}

utf::StringView ast::VariableDeclaration::getName() const noexcept {
    return m_name;
}

const ast::Type& ast::VariableDeclaration::getVariableType() const noexcept {
    return m_type;
}

utils::NoNull<ast::Expression>& ast::VariableDeclaration::getInitialValue() noexcept {
    return m_initialValue;
}
