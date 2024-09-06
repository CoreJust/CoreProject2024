// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirUnaryOperator.hpp"

chir::UnaryOperator::UnaryOperator(UnaryOperatorType operatorType, utils::NoNull<Value> value, utils::NoNull<symbol::Type> type) noexcept
	: Value(NodeKind::UNARY_OPERATOR, type), m_operatorType(operatorType), m_value(value) {
	Node::setParent(m_value, this);
}

chir::UnaryOperator::~UnaryOperator() {
	m_value->~Value();
}

utils::NoNull<chir::Value>& chir::UnaryOperator::getValue() noexcept {
	return m_value;
}

chir::UnaryOperator::UnaryOperatorType chir::UnaryOperator::getOperator() const noexcept {
	return m_operatorType;
}
