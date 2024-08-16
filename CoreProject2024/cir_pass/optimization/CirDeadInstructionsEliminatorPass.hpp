// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirDeadInstructionsEliminatorPass.hpp by CoreJust
*	Created on 16.08.2024
*	Contains DeadInstructionsEliminatorPass class that is derived from ModulePass.
* 
*	This pass removes all the basic blocks without predecessors (apart from the entry block),
*	and removes instructions after the terminator.
*	It is required because further passes may break otherwise.
*/

#pragma once
#include "../CirPass.hpp"

namespace cir_pass {
	class DeadInstructionsEliminatorPass final : public CommonFunctionPass {
	public:
		void pass(utils::NoNull<cir::CommonFunction> function) override;

	private:
		void removeInstructionAfterTerminator(utils::NoNull<cir::BasicBlock> basicBlock);

		// Returns true is there was at least one successful deletion.
		bool tryToRemoveDeadBasicBlocks(utils::NoNull<cir::CommonFunction> function);
	};
}