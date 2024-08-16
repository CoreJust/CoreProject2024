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
#include <list>
#include "CirConstant.hpp"
#include "../instruction/CirInstruction.hpp"

namespace cir {
	class CommonFunction;
	
	class BasicBlock final : public Constant {
	private:
		CommonFunction& m_parentFunction;
		std::list<utils::NoNull<Instruction>> m_instructions;

		// The edges that form the Control Flow Graph.
		std::vector<utils::NoNull<BasicBlock>> m_predecessors;
		std::vector<utils::NoNull<BasicBlock>> m_successors;

		uint64_t& m_instructionCounter;

	public:
		BasicBlock(CommonFunction& parentFunction, utf::String name) noexcept;

		// Adds the instruction to the end of BasicBlock's instruction list.
		void addInstruction(utils::NoNull<Instruction> instruction) noexcept;

		// Removes the instruction from the BasicBlock's instruction list.
		// Updates the CFG.
		std::list<utils::NoNull<Instruction>>::iterator removeInstruction(std::list<utils::NoNull<Instruction>>::iterator it);

		// Adds the basic block to this one's successors
		// and adds this one to the basic block predecessors.
		void addSuccessor(utils::NoNull<BasicBlock> successor);

		// Removes the basic block from this one's successors
		// and removes this one from the basic block predecessors.
		void removeSuccessor(utils::NoNull<BasicBlock> successor);

		CommonFunction& getParentFunction() noexcept;
		std::list<utils::NoNull<Instruction>>& getInstructions() noexcept;
		std::vector<utils::NoNull<BasicBlock>>& getPredecessors() noexcept;
		std::vector<utils::NoNull<BasicBlock>>& getSuccessors() noexcept;

		// Returns true if the last instruction of the basic block is a terminator.
		bool hasTerminator() const noexcept;
	};
}
