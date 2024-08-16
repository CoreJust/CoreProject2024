// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirBranchInstruction.hpp"
#include <format>

cir::BranchInstruction::BranchInstruction(utils::NoNull<Value> condition, utils::NoNull<BasicBlock> successBranch, utils::NoNull<BasicBlock> failureBranch) noexcept
    : Terminator("", TypeKind::UNIT, ValueKind::BRANCH_INSTRUCTION), m_condition(condition), m_successBranch(successBranch), m_failureBranch(failureBranch) {
    error::internalAssert(m_condition->getType() == TypeKind::BOOL);
}

cir::Value*& cir::BranchInstruction::getCondition() noexcept {
    return m_condition;
}

utils::NoNull<cir::BasicBlock>& cir::BranchInstruction::getSuccessBranch() noexcept {
    return m_successBranch;
}

utils::NoNull<cir::BasicBlock>& cir::BranchInstruction::getFailureBranch() noexcept {
    return m_failureBranch;
}

utf::String cir::BranchInstruction::toInstuctionString() const {
    return std::format("branch {} => {} else {}", m_condition->toString(), m_successBranch->toString(), m_failureBranch->toString());
}
