// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirInvocationInstruction.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "../constant/CirCommonFunction.hpp"
#include "cir/type/CirFunctionType.hpp"

cir::InvocationInstruction::InvocationInstruction(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, utf::String name) noexcept
	: Instruction(std::move(name), callee->getType().as<FunctionType>()->getReturnType(), ValueKind::INVOCATION_INSTRUCTION),
	m_callee(callee),
	m_arguments(std::move(arguments)) 
{
	Value::addUser(m_callee, *this);
	for (auto argument : m_arguments) {
		Value::addUser(argument, *this);
	}
}

utils::NoNull<cir::Value>& cir::InvocationInstruction::getCallee() noexcept {
	return m_callee;
}

std::vector<utils::NoNull<cir::Value>>& cir::InvocationInstruction::getArguments() noexcept {
	return m_arguments;
}

utf::String cir::InvocationInstruction::toInstuctionString() const {
	return std::format("tmp {} = call {} ({})", m_name, m_callee->toString(), utils::joinToString(m_arguments));
}
