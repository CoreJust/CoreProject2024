// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirUnaryInstruction.hpp"

cir::UnaryInstruction::UnaryInstruction(UnaryInstructionKind instructionKind, utils::NoNull<Value> operand, utf::String name) noexcept
	: Instruction(std::move(name), operand->getType(), ValueKind::UNARY_INSTRUCTION), m_instuctionKind(instructionKind), m_operand(operand) {
	Value::addUser(m_operand, *this);
}

cir::UnaryInstruction::UnaryInstructionKind cir::UnaryInstruction::getInstructionKind() const noexcept {
	return m_instuctionKind;
}

utils::NoNull<cir::Value>& cir::UnaryInstruction::getOperand() noexcept {
	return m_operand;
}

utf::String cir::UnaryInstruction::toInstuctionString() const {
	return std::format("tmp {} = {} {}", m_name, "-!"[m_instuctionKind], m_operand->toString());
}
