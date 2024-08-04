// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ExpressionStatement.hpp"

ast::ExpressionStatement::ExpressionStatement(utils::NoNull<ast::Expression> expression) noexcept
    : Statement(NodeType::EXPRESSION_STATEMENT), m_expression(expression) {
    Node::setParent(m_expression, this);
}

utils::NoNull<ast::Expression>& ast::ExpressionStatement::getExpression() noexcept {
    return m_expression;
}
