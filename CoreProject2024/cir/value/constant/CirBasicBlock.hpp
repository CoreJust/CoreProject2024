// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirBasicBlock.hpp by CoreJust
*	Created on 05.08.2024
*	Contains BasicBlock class that is derived from Constant
*	and represents a basic block of instructions in CIR
*	that ends with a terminator instruction.
*/

#pragma once
#include "CirConstant.hpp"
#include "../instruction/CirInstruction.hpp"

namespace cir {
	class CommonFunction;
	
	class BasicBlock final : public Constant {
	private:
		CommonFunction& m_parentFunction;
		std::vector<utils::NoNull<Instruction>> m_instructions;

		// The edges that form the Control Flow Graph.
		std::vector<utils::NoNull<BasicBlock>> m_predecessors;
		std::vector<utils::NoNull<BasicBlock>> m_successors;

		uint64_t& m_instructionCounter;

	public:
		BasicBlock(CommonFunction& parentFunction, utf::String name) noexcept;

		// Adds the instruction to the end of BasicBlock's instruction list.
		void addInstruction(utils::NoNull<Instruction> instruction) noexcept;

		CommonFunction& getParentFunction() noexcept;
		std::vector<utils::NoNull<Instruction>>& getInstructions() noexcept;
		std::vector<utils::NoNull<BasicBlock>>& getPredecessors() noexcept;
		std::vector<utils::NoNull<BasicBlock>>& getSuccessors() noexcept;

		// Returns true if the last instruction of the basic block is a terminator.
		bool hasTerminator() const noexcept;
	};
}
