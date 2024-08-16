// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "IfElseStatement.hpp"

ast::IfElseStatement::IfElseStatement(std::vector<utils::NoNull<ast::Expression>> conditions, std::vector<utils::NoNull<ast::Statement>> ifBodies, Statement* elseBody) noexcept
    : Statement(NodeKind::IF_ELSE_STATEMENT), m_conditions(std::move(conditions)), m_ifBodies(std::move(ifBodies)), m_elseBody(elseBody) {
    error::internalAssert(m_conditions.size() == m_ifBodies.size());
    error::internalAssert(!m_conditions.empty());

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

std::vector<utils::NoNull<ast::Expression>>& ast::IfElseStatement::getConditions() noexcept {
    return m_conditions;
}

std::vector<utils::NoNull<ast::Statement>>& ast::IfElseStatement::getIfBodies() noexcept {
    return m_ifBodies;
}

ast::Statement*& ast::IfElseStatement::getElseBody() noexcept {
    return m_elseBody;
}

bool ast::IfElseStatement::hasElse() const noexcept {
    return m_elseBody != nullptr;
}
