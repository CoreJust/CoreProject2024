// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirUnaryInstruction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains UnaryInstruction class that is derived from Instruction
*	and represents an operation with a single operand and a return value
*	in CIR.
*/

#pragma once
#include "CirInstruction.hpp"

namespace cir {
	class UnaryInstruction final : public Instruction {
	public:
		enum UnaryInstructionKind : unsigned char {
			NEG = 0,
			BITWISE_NOT,
			LOGIC_NOT,
			CAST,

			UNARY_INSTRUCTIONS_COUNT
		};

	private:
		utils::NoNull<Value> m_operand;
		UnaryInstructionKind m_instuctionKind;

	public:
		UnaryInstruction(UnaryInstructionKind instructionKind, utils::NoNull<Value> operand, utf::String name = "u") noexcept;
		UnaryInstruction(utils::NoNull<Value> operand, utils::NoNull<cir::Type> type, utf::String name = "u") noexcept; // Type cast
		~UnaryInstruction() override;

		UnaryInstructionKind getInstructionKind() const noexcept;
		utils::NoNull<Value>& getOperand() noexcept;

		utf::String toInstuctionString() const override;
	};
}