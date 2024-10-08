// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirBinaryOperator.hpp"

chir::BinaryOperator::BinaryOperator(BinaryOperatorType operatorType, utils::NoNull<Value> left, utils::NoNull<Value> right, utils::NoNull<symbol::Type> type) noexcept
	: Value(NodeKind::BINARY_OPERATOR, type), m_operatorType(operatorType), m_left(left), m_right(right) {
	Node::setParent(m_left, this);
	Node::setParent(m_right, this);
}

chir::BinaryOperator::~BinaryOperator() {
	m_left->~Value();
	m_right->~Value();
}

utils::NoNull<chir::Value>& chir::BinaryOperator::getLeft() noexcept {
	return m_left;
}

utils::NoNull<chir::Value>& chir::BinaryOperator::getRight() noexcept {
	return m_right;
}

chir::BinaryOperator::BinaryOperatorType chir::BinaryOperator::getOperator() const noexcept {
	return m_operatorType;
}
