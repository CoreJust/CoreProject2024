// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirBasicBlock.hpp"
#include <format>
#include "error/ErrorPrinter.hpp"
#include "CirCommonFunction.hpp"
#include "cir/value/instruction/CirGotoInstruction.hpp"
#include "cir/value/instruction/CirBranchInstruction.hpp"

cir::BasicBlock::BasicBlock(CommonFunction& parentFunction, utf::String name) noexcept
	: Constant(std::move(name), Type::make(TypeKind::BASIC_BLOCK), ValueKind::BASIC_BLOCK), 
	m_parentFunction(parentFunction),
	m_instructionCounter(parentFunction.getInstructionCounter()) {

}

cir::BasicBlock::~BasicBlock() {
	for (auto instruction : m_instructions) {
		instruction->~Instruction();
	}
}

void cir::BasicBlock::addInstruction(utils::NoNull<Instruction> instruction) noexcept {
	m_instructions.emplace_back(instruction);
	if (instruction->getKind() != ValueKind::LOCAL_VARIABLE) {
		instruction->appendInstructionIndexToName(m_instructionCounter++);
	}
}

std::list<utils::NoNull<cir::Instruction>>::iterator cir::BasicBlock::removeInstruction(std::list<utils::NoNull<Instruction>>::iterator it) {
	utils::NoNull<cir::Instruction> instruction = *it;
	switch (instruction->getKind()) {
		case ValueKind::GOTO_INSTRUCTION: removeSuccessor(instruction.as<cir::GotoInstruction>()->getBasicBlockToGo()); break;
		case ValueKind::BRANCH_INSTRUCTION: 
			removeSuccessor(instruction.as<cir::BranchInstruction>()->getSuccessBranch()); 
			removeSuccessor(instruction.as<cir::BranchInstruction>()->getFailureBranch()); 
			break;
	default: break;
	}

	return m_instructions.erase(it);
}

void cir::BasicBlock::addSuccessor(utils::NoNull<BasicBlock> successor) {
	m_successors.emplace_back(successor);
	successor->m_predecessors.emplace_back(this);
}

void cir::BasicBlock::removeSuccessor(utils::NoNull<BasicBlock> successor) {
	std::erase(m_successors, successor);
	std::erase(successor->m_predecessors, utils::NoNull<BasicBlock>(this));
}

cir::CommonFunction& cir::BasicBlock::getParentFunction() noexcept {
	return m_parentFunction;
}

std::list<utils::NoNull<cir::Instruction>>& cir::BasicBlock::getInstructions() noexcept {
	return m_instructions;
}

std::vector<utils::NoNull<cir::BasicBlock>>& cir::BasicBlock::getPredecessors() noexcept {
	return m_predecessors;
}

std::vector<utils::NoNull<cir::BasicBlock>>& cir::BasicBlock::getSuccessors() noexcept {
	return m_successors;
}

bool cir::BasicBlock::hasTerminator() const noexcept {
	return !m_instructions.empty() && m_instructions.back()->isTerminator();
}
