// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "InvocationOperator.hpp"

ast::InvocationOperator::InvocationOperator(utils::NoNull<Expression> callee, std::vector<utils::NoNull<Expression>> arguments) noexcept
	: Expression(NodeKind::INVOCATION_OPERATOR), m_callee(callee), m_arguments(std::move(arguments)) {
	// Setting the current node as the parent.
	Node::setParent(m_callee, this);
	for (auto argument : m_arguments) {
		Node::setParent(argument, this);
	}
}

utils::NoNull<ast::Expression>& ast::InvocationOperator::getCallee() noexcept {
	return m_callee;
}

std::vector<utils::NoNull<ast::Expression>>& ast::InvocationOperator::getArguments() noexcept {
	return m_arguments;
}
