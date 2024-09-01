// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AsOperator.hpp"

ast::AsOperator::AsOperator(utils::NoNull<Expression> value, utils::NoNull<Type> type) noexcept
    : Expression(NodeKind::AS_OPERATOR), m_value(value), m_type(type) {
    Node::setParent(m_value, this);
}

utils::NoNull<ast::Expression>& ast::AsOperator::getValue() noexcept {
    return m_value;
}

utils::NoNull<ast::Type>& ast::AsOperator::getType() noexcept {
    return m_type;
}
