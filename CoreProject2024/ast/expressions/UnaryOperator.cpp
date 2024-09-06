// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "UnaryOperator.hpp"

ast::UnaryOperator::UnaryOperator(UnaryOperatorType operatorType, utils::NoNull<Expression> expression) noexcept
	: Expression(NodeKind::UNARY_OPERATOR), m_operator(operatorType), m_expression(expression) {
	Node::setParent(m_expression, this);
}

ast::UnaryOperator::~UnaryOperator() {
	m_expression->~Expression();
}

utils::NoNull<ast::Expression>& ast::UnaryOperator::getExpression() noexcept {
	return m_expression;
}

ast::UnaryOperator::UnaryOperatorType ast::UnaryOperator::getOperator() const noexcept {
	return m_operator;
}

bool ast::UnaryOperator::isArithmetical() const noexcept {
	return m_operator <= UnaryOperatorType::MINUS;
}

bool ast::UnaryOperator::isLogical() const noexcept {
	return m_operator > UnaryOperatorType::MINUS;
}
