// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ComparativeBinaryOperator.hpp"
#include "error/InternalAssert.hpp"

ast::ComparativeBinaryOperator::ComparativeBinaryOperator(std::vector<ComparativeOperatorType> operators, std::vector<utils::NoNull<Expression>> expressions) noexcept
	: Expression(NodeKind::COMPARATIVE_BINARY_OPERATOR), m_operators(std::move(operators)), m_expressions(std::move(expressions)) {
	internalAssert(!m_operators.empty(), "There must be at least one comparison in ComparativeBinaryOperator");
	internalAssert(m_operators.size() + 1 == m_expressions.size(), "Operators and expressions count mismatch");

	for (auto expression : m_expressions) {
		Node::setParent(expression, this);
	}
}

ast::ComparativeBinaryOperator::~ComparativeBinaryOperator() {
	for (auto expression : m_expressions) {
		expression->~Expression();
	}
}

std::vector<utils::NoNull<ast::Expression>>& ast::ComparativeBinaryOperator::getExpressions() noexcept {
	return m_expressions;
}

std::vector<ast::ComparativeBinaryOperator::ComparativeOperatorType>& ast::ComparativeBinaryOperator::getOperators() noexcept {
	return m_operators;
}
