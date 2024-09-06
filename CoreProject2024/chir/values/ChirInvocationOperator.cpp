// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirInvocationOperator.hpp"

chir::InvocationOperator::InvocationOperator(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, utils::NoNull<symbol::Type> type) noexcept
	: Value(NodeKind::INVOCATION_OPERATOR, type), m_callee(callee), m_arguments(std::move(arguments)) {

	// Setting the current node as the parent.
	Node::setParent(m_callee, this);
	for (auto argument : m_arguments) {
		Node::setParent(argument, this);
	}
}

chir::InvocationOperator::~InvocationOperator() {
	m_callee->~Value();
	for (auto argument : m_arguments) {
		argument->~Value();
	}
}

utils::NoNull<chir::Value>& chir::InvocationOperator::getCallee() noexcept {
	return m_callee;
}

std::vector<utils::NoNull<chir::Value>>& chir::InvocationOperator::getArguments() noexcept {
	return m_arguments;
}