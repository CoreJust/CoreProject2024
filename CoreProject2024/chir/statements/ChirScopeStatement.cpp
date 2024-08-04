// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirScopeStatement.hpp"

chir::ScopeStatement::ScopeStatement(std::vector<utils::NoNull<Statement>> statements) noexcept
    : Statement(NodeType::SCOPE_STATEMENT), m_statements(std::move(statements)) {
    for (auto statement : m_statements) {
        Node::setParent(statement, this);
    }
}

const std::vector<utils::NoNull<chir::Statement>>& chir::ScopeStatement::getStatements() const noexcept {
    return m_statements;
}
