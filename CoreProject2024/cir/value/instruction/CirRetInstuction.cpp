// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirRetInstuction.hpp"
#include <format>

cir::RetInstruction::RetInstruction() noexcept 
	: Terminator("", Type::make(TypeKind::UNIT), ValueKind::RET_INSTRUCTION), m_operand(nullptr) {

}

cir::RetInstruction::RetInstruction(utils::NoNull<Value> operand) noexcept
	: Terminator("", Type::make(TypeKind::UNIT), ValueKind::RET_INSTRUCTION), m_operand(operand) {
	Value::addUser(m_operand, *this);
}

bool cir::RetInstruction::isRetUnit() const noexcept {
	return m_operand == nullptr;
}

cir::Value*& cir::RetInstruction::getOperand() noexcept {
	return m_operand;
}

utils::NoNull<cir::Type> cir::RetInstruction::getReturnType() noexcept {
	static utils::NoNull<Type> s_unitType = Type::make(TypeKind::UNIT);

	if (m_operand == nullptr) {
		return s_unitType;
	} else {
		return m_operand->getType();
	}
}

utf::String cir::RetInstruction::toInstuctionString() const {
	if (m_operand != nullptr) {
		return std::format("ret {}", m_operand->toString());
	} else {
		return "ret unit";
	}
}
