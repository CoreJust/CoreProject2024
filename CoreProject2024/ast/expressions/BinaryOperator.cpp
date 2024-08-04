// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "BinaryOperator.hpp"

ast::BinaryOperator::BinaryOperator(BinaryOperatorType operatorType, utils::NoNull<Expression> left, utils::NoNull<Expression> right) noexcept
    : Expression(NodeType::BINARY_OPERATOR), m_operator(operatorType), m_left(left), m_right(right) {
    Node::setParent(m_left, this);
    Node::setParent(m_right, this);
}

utils::NoNull<ast::Expression>& ast::BinaryOperator::getLeft() noexcept {
    return m_left;
}

utils::NoNull<ast::Expression>& ast::BinaryOperator::getRight() noexcept {
    return m_right;
}

ast::BinaryOperator::BinaryOperatorType ast::BinaryOperator::getOperator() const noexcept {
    return m_operator;
}
