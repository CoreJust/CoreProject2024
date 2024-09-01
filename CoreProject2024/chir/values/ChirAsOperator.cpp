// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirAsOperator.hpp"

chir::AsOperator::AsOperator(utils::NoNull<Value> value, utils::NoNull<symbol::Type> type) noexcept 
	: Value(NodeKind::AS_OPERATOR, type), m_value(value) {
	Node::setParent(m_value, this);
}

utils::NoNull<chir::Value>& chir::AsOperator::getValue() noexcept {
	return m_value;
}