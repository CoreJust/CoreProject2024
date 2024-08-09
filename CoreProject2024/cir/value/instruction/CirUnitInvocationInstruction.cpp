// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirUnitInvocationInstruction.hpp"
#include "utils/CollectionUtils.hpp"

cir::UnitInvocationInstruction::UnitInvocationInstruction(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments) noexcept
	: Instruction("", TypeKind::UNIT, ValueKind::UNIT_INVOCATION_INSTRUCTION),
	m_callee(callee),
	m_arguments(std::move(arguments)) 
{
	Value::addUser(m_callee, *this);
	for (auto argument : m_arguments) {
		Value::addUser(argument, *this);
	}
}

utils::NoNull<cir::Value>& cir::UnitInvocationInstruction::getCallee() noexcept {
	return m_callee;
}

std::vector<utils::NoNull<cir::Value>>& cir::UnitInvocationInstruction::getArguments() noexcept {
	return m_arguments;
}

utf::String cir::UnitInvocationInstruction::toInstuctionString() const {
	return std::format("unit call {} ({})", m_callee->toString(), utils::joinToString(m_arguments));
}
