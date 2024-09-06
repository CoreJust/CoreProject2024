// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirLocalVariable.hpp"
#include <format>

cir::LocalVariable::LocalVariable(utf::String name, utils::NoNull<Type> type, utils::NoNull<Value> initialValue) noexcept
	: Instruction(std::move(name), type, ValueKind::LOCAL_VARIABLE), m_initialValue(initialValue) {
	internalAssert(m_type->getTypeKind() != TypeKind::UNIT);
	internalAssert(m_type->equals(m_initialValue->getType()));

	Value::addUser(m_initialValue, *this);
}

cir::LocalVariable::~LocalVariable() {
	Instruction::destroyIfConstant(m_initialValue);
}

utils::NoNull<cir::Value>& cir::LocalVariable::getInitialValue() noexcept {
	return m_initialValue;
}

utf::String cir::LocalVariable::toInstuctionString() const {
	return std::format("let {}: {} = {}", m_name, m_type->toString(), m_initialValue->toString());
}
