// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirReturnOperator.hpp"

chir::ReturnOperator::ReturnOperator() noexcept
	: Value(NodeKind::RETURN_OPERATOR, symbol::Type::make(symbol::TypeKind::NEVER_TYPE)), m_value(nullptr) {
}

chir::ReturnOperator::ReturnOperator(utils::NoNull<Value> value) noexcept
	: Value(NodeKind::RETURN_OPERATOR, symbol::Type::make(symbol::TypeKind::NEVER_TYPE)), m_value(value) {
	Node::setParent(m_value, this);
}

chir::Value*& chir::ReturnOperator::getValue() noexcept {
	return m_value;
}
