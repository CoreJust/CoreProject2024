// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirIfElseStatement.hpp"

chir::IfElseStatement::IfElseStatement(std::vector<utils::NoNull<chir::Value>> conditions, std::vector<utils::NoNull<chir::Statement>> ifBodies, Statement* elseBody) noexcept
	: Statement(NodeKind::IF_ELSE_STATEMENT), m_conditions(std::move(conditions)), m_ifBodies(std::move(ifBodies)), m_elseBody(elseBody) {
	internalAssert(m_conditions.size() == m_ifBodies.size());
	internalAssert(!m_conditions.empty());

	for (auto condition : m_conditions) {
		Node::setParent(condition, this);
	}

	for (auto ifBody : m_ifBodies) {
		Node::setParent(ifBody, this);
	}

	if (m_elseBody != nullptr) {
		Node::setParent(m_elseBody, this);
	}
}

chir::IfElseStatement::~IfElseStatement() {
	for (auto condition : m_conditions) {
		condition->~Value();
	}

	for (auto ifBody : m_ifBodies) {
		ifBody->~Statement();
	}

	if (m_elseBody != nullptr) {
		m_elseBody->~Statement();
	}
}

std::vector<utils::NoNull<chir::Value>>& chir::IfElseStatement::getConditions() noexcept {
	return m_conditions;
}

std::vector<utils::NoNull<chir::Statement>>& chir::IfElseStatement::getIfBodies() noexcept {
	return m_ifBodies;
}

chir::Statement*& chir::IfElseStatement::getElseBody() noexcept {
	return m_elseBody;
}

bool chir::IfElseStatement::hasElse() const noexcept {
	return m_elseBody != nullptr;
}
