// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirBasicBlock.hpp"
#include "error/ErrorPrinter.hpp"
#include "CirCommonFunction.hpp"

cir::BasicBlock::BasicBlock(CommonFunction& parentFunction, utf::String name) noexcept
    : Constant(std::move(name), TypeKind::BASIC_BLOCK, ValueKind::BASIC_BLOCK), 
    m_parentFunction(parentFunction),
    m_instructionCounter(parentFunction.getInstructionCounter()) {

}

void cir::BasicBlock::addInstruction(utils::NoNull<Instruction> instruction) noexcept {
    if (!m_instructions.empty() && m_instructions.back()->isTerminator()) {
        error::ErrorPrinter::error({
            .code = error::ErrorCode::INSTRUCTION_AFTER_TERMINATOR,
            .name = "CIR error: Instruction after a terminator",
            .selectionStart = utils::TextPosition(),
            .selectionLength = 0,
            .description = std::format("Tried to append an instruction after a terminator."),
            .explanation = "-"
        });

        assert(false); // For debug mode.
    }

    m_instructions.emplace_back(instruction);
    instruction->appendInstructionIndexToName(m_instructionCounter++);
}

cir::CommonFunction& cir::BasicBlock::getParentFunction() noexcept {
    return m_parentFunction;
}

std::vector<utils::NoNull<cir::Instruction>>& cir::BasicBlock::getInstructions() noexcept {
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
