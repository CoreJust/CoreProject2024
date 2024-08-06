// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirInstruction.hpp"
#include <cassert>

cir::Instruction::Instruction(utf::String name, Type type, ValueKind kind) noexcept 
	: Value(std::move(name), std::move(type), kind) {
	assert(cir::isIntruction(kind));
}

void cir::Instruction::appendInstructionIndexToName(uint64_t index) {
	m_name += std::to_string(index);
}
