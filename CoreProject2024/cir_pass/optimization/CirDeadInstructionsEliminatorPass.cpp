// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirDeadInstructionsEliminatorPass.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"

void cir_pass::DeadInstructionsEliminatorPass::pass(utils::NoNull<cir::CommonFunction> function) {
	// Removing instructions after the first terminator in each basic block.
	for (auto basicBlock : function->getBasicBlocks()) {
		removeInstructionAfterTerminator(basicBlock);
	}

	// Removing unreachable basic blocks.
	while (tryToRemoveDeadBasicBlocks(function));
}

void cir_pass::DeadInstructionsEliminatorPass::removeInstructionAfterTerminator(utils::NoNull<cir::BasicBlock> basicBlock) {
	std::list<utils::NoNull<cir::Instruction>>& instructions = basicBlock->getInstructions();

	auto it = instructions.begin();
	
	// Looking for the first terminator.
	while (it != instructions.end() && !(*it)->isTerminator()) ++it;

	if (it != instructions.end()) {
		++it; // Skip the first terminator itself.

		// Removing all the instructions after the first terminator.
		while (it != instructions.end()) {
			it = basicBlock->removeInstruction(it);
		}
	}
}

bool cir_pass::DeadInstructionsEliminatorPass::tryToRemoveDeadBasicBlocks(utils::NoNull<cir::CommonFunction> function) {
	bool modified = false;

	std::list<utils::NoNull<cir::BasicBlock>>& basicBlocks = function->getBasicBlocks();
	auto it = basicBlocks.begin();
	++it; // Skipping the entry basic block since it has an implicit predecessor.
	for (; it != basicBlocks.end(); ) {
		if ((*it)->getPredecessors().empty()) {
			// Removing basic blocks without any predecessors.
			it = function->removeBasicBlock(it);
			modified = true;
		} else {
			++it;
		}
	}

	return modified;
}
