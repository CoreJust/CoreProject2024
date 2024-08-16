// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirBinaryInstruction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains BinaryInstruction class that is derived from Instruction
*	and represents an operation with 2 operands and a return value
*	in CIR.
*/

#pragma once
#include "CirInstruction.hpp"

namespace cir {
	class BinaryInstruction final : public Instruction {
	public:
		enum BinaryInstructionKind : unsigned char {
			ADD = 0,
			SUB,
			MUL,
			DIV,
			REM,

			LOGIC_AND,
			LOGIC_OR,

			CMP_EQ,
			CMP_NEQ,
			CMP_LEQ,
			CMP_GEQ,
			CMP_LT,
			CMP_GT,

			BINARY_INSTRUCTIONS_COUNT
		};

	private:
		utils::NoNull<Value> m_left;
		utils::NoNull<Value> m_right;
		BinaryInstructionKind m_instuctionKind;

	public:
		BinaryInstruction(BinaryInstructionKind instructionKind, utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b") noexcept;

		BinaryInstructionKind getInstructionKind() const noexcept;
		utils::NoNull<Value>& getLeft() noexcept;
		utils::NoNull<Value>& getRight() noexcept; 
		
		utf::String toInstuctionString() const override;
	};
}