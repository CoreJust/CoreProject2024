// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirCommonFunction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains CommonFunction class that is derived from Function 
*	and represents a function with implementation in CIR.
*/

#pragma once
#include <vector>
#include <list>
#include "CirFunction.hpp"
#include "CirBasicBlock.hpp"

namespace cir {
	class CommonFunction final : public Function {
	private:
		std::list<utils::NoNull<BasicBlock>> m_basicBlocks;
		uint64_t m_instructionCounter; // Used for unique naming of instructions.

	public:
		CommonFunction(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, utils::NoNull<Module> parentModule) noexcept;

		utils::NoNull<BasicBlock> makeBasicBlock(utf::String name) noexcept;

		// Removes the basic block from basic blocks list and modifies CFG.
		std::list<utils::NoNull<BasicBlock>>::iterator removeBasicBlock(std::list<utils::NoNull<BasicBlock>>::iterator it);

		std::list<utils::NoNull<BasicBlock>>& getBasicBlocks() noexcept;
		uint64_t& getInstructionCounter() noexcept;
	};
}