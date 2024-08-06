// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ReturnOperator.hpp"

ast::ReturnOperator::ReturnOperator() noexcept
    : Expression(NodeKind::RETURN_OPERATOR), m_expression(nullptr) { }

ast::ReturnOperator::ReturnOperator(utils::NoNull<Expression> expression) noexcept
    : Expression(NodeKind::RETURN_OPERATOR), m_expression(expression.get()) {
    Node::setParent(m_expression, this);
}

ast::Expression*& ast::ReturnOperator::getExpression() noexcept {
    return m_expression;
}
