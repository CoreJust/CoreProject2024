#include "InvocationOperator.hpp"

ast::InvocationOperator::InvocationOperator(Expression* callee, std::vector<Expression*> arguments) noexcept
	: Expression(NodeType::INVOCATION_OPERATOR), m_callee(callee), m_arguments(std::move(arguments)) {
	// Setting the current node as the parent.
	Node::setParent(m_callee, this);
	for (auto argument : m_arguments) {
		Node::setParent(argument, this);
	}
}

ast::Expression*& ast::InvocationOperator::getCallee() noexcept {
	return m_callee;
}

const std::vector<ast::Expression*>& ast::InvocationOperator::getArguments() noexcept {
	return m_arguments;
}
