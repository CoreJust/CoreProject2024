// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirBinaryInstruction.hpp"
#include <format>

cir::BinaryInstruction::BinaryInstruction(BinaryInstructionKind instructionKind, utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) noexcept
	: Instruction(std::move(name), instructionKind >= BinaryInstructionKind::LOGIC_AND ? Type::make(TypeKind::BOOL) : left->getType(), ValueKind::BINARY_INSTRUCTION),
	m_instuctionKind(instructionKind), m_left(left), m_right(right) 
{
	Value::addUser(m_left, *this);
	Value::addUser(m_right, *this);
}

cir::BinaryInstruction::~BinaryInstruction() {
	Instruction::destroyIfConstant(m_left);
	Instruction::destroyIfConstant(m_right);
}

cir::BinaryInstruction::BinaryInstructionKind cir::BinaryInstruction::getInstructionKind() const noexcept {
	return m_instuctionKind;
}

utils::NoNull<cir::Value>& cir::BinaryInstruction::getLeft() noexcept {
	return m_left;
}

utils::NoNull<cir::Value>& cir::BinaryInstruction::getRight() noexcept {
	return m_right;
}

utf::String cir::BinaryInstruction::toInstuctionString() const {
	static const char* OPERATORS[] = { "+", "-", "*", "/", "%", "&&", "||", "==", "!=", "<=", ">=", "<", ">" };

	return std::format("tmp {} = {} {} {}", m_name, m_left->toString(), OPERATORS[m_instuctionKind], m_right->toString());
}
