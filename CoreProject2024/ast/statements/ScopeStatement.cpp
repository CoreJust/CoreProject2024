// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ScopeStatement.hpp"

ast::ScopeStatement::ScopeStatement(std::vector<Statement*> statements) noexcept
    : Statement(NodeType::SCOPE_STATEMENT), m_statements(std::move(statements)) {
}

const std::vector<ast::Statement*>& ast::ScopeStatement::getStatements() noexcept {
    return m_statements;
}
