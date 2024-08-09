// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirTerminator.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Terminator class that is derived from Instruction
*	and represents a terminator instruction in CIR.
*/

#pragma once
#include "CirInstruction.hpp"

namespace cir {
	class Terminator : public Instruction {
	public:
		Terminator(utf::String name, Type type, ValueKind kind) noexcept;
	};
}