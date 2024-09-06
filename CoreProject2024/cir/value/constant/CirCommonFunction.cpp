// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirCommonFunction.hpp"
#include "cir/CirAllocator.hpp"

cir::CommonFunction::CommonFunction(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, utils::NoNull<Module> parentModule) noexcept
	: Function(std::move(name), returnType, std::move(arguments), ValueKind::COMMON_FUNCTION, parentModule),
	m_instructionCounter(0) {

}

cir::CommonFunction::~CommonFunction() {
	for (auto basicBlock : m_basicBlocks) {
		basicBlock->~BasicBlock();
	}
}

utils::NoNull<cir::BasicBlock> cir::CommonFunction::makeBasicBlock(utf::String name) noexcept {
	return m_basicBlocks.emplace_back(cir::CirAllocator::make<cir::BasicBlock>(*this, name + std::to_string(m_basicBlocks.size())));
}

std::list<utils::NoNull<cir::BasicBlock>>::iterator cir::CommonFunction::removeBasicBlock(std::list<utils::NoNull<BasicBlock>>::iterator it) {
	std::list<utils::NoNull<Instruction>>& instructions = (*it)->getInstructions();
	for (auto instructionIt = instructions.begin(); instructionIt != instructions.end();) {
		if ((*instructionIt)->isTerminator()) {
			instructionIt = (*it)->removeInstruction(instructionIt);
		} else {
			++instructionIt;
		}
	}

	return m_basicBlocks.erase(it);
}

std::list<utils::NoNull<cir::BasicBlock>>& cir::CommonFunction::getBasicBlocks() noexcept {
	return m_basicBlocks;
}

uint64_t& cir::CommonFunction::getInstructionCounter() noexcept {
	return m_instructionCounter;
}
