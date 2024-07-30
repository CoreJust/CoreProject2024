// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "UnaryOperator.hpp"

ast::UnaryOperator::UnaryOperator(UnaryOperatorType operatorType, Expression* expression) noexcept 
	: Expression(NodeType::UNARY_OPERATOR), m_operator(operatorType), m_expression(expression) {
	Node::setParent(m_expression, this);
}

ast::Expression*& ast::UnaryOperator::getExpression() noexcept {
	return m_expression;
}

ast::UnaryOperator::UnaryOperatorType ast::UnaryOperator::getOperator() const noexcept {
	return m_operator;
}
