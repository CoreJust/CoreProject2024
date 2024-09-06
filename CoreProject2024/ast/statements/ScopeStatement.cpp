// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ScopeStatement.hpp"

ast::ScopeStatement::ScopeStatement(std::vector<utils::NoNull<ast::Statement>> statements) noexcept
	: Statement(NodeKind::SCOPE_STATEMENT), m_statements(std::move(statements)) {
	for (auto statement : m_statements) {
		Node::setParent(statement, this);
	}
}

ast::ScopeStatement::~ScopeStatement() {
	for (auto statement : m_statements) {
		statement->~Statement();
	}
}

const std::vector<utils::NoNull<ast::Statement>>& ast::ScopeStatement::getStatements() noexcept {
	return m_statements;
}
