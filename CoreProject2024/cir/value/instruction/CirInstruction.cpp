// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirInstruction.hpp"
#include "cir/value/constant/CirConstanNumber.hpp"
#include "error/InternalAssert.hpp"
#include "utf/FastFmt.hpp"

void cir::Instruction::destroyIfConstant(utils::NoNull<Value> value) {
	if (value->getKind() == ValueKind::CONSTANT_NUMBER) {
		value.as<ConstantNumber>()->~ConstantNumber();
	}
}

cir::Instruction::Instruction(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept
	: Value(std::move(name), type, kind) {
	internalAssert(cir::isIntruction(kind));
}

void cir::Instruction::appendInstructionIndexToName(uint64_t index) {
	m_name.append(utf::fastToString(index));
}
