// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirInstruction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Instruction class that is derived from Value
*	and represents a single instruction in CIR.
*/

#pragma once
#include "../CirValue.hpp"

namespace cir {
	class Instruction : public Value {
	public:
		Instruction(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept;

		// Appends the index of the instruction within a function to instruction name.
		void appendInstructionIndexToName(uint64_t index);

		// Unlike Value.toString(), this one returns the string representation
		// of this instruction and not of its use.
		virtual utf::String toInstuctionString() const = 0;
	};
}