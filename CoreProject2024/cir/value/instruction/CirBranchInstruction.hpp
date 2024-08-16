// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirBranchInstruction.hpp by CoreJust
*	Created on 15.08.2024
*	Contains BranchInstruction class that is derived from Terminator
*	and represents a branching instruction in CIR.
*/

#pragma once
#include "CirTerminator.hpp"
#include "cir/value/constant/CirBasicBlock.hpp"

namespace cir {
	class BranchInstruction final : public Terminator {
	private:
		Value* m_condition;
		utils::NoNull<BasicBlock> m_successBranch;
		utils::NoNull<BasicBlock> m_failureBranch;

	public:
		BranchInstruction(utils::NoNull<Value> condition, utils::NoNull<BasicBlock> successBranch, utils::NoNull<BasicBlock> failureBranch) noexcept;

		Value*& getCondition() noexcept;
		utils::NoNull<BasicBlock>& getSuccessBranch() noexcept;
		utils::NoNull<BasicBlock>& getFailureBranch() noexcept;

		utf::String toInstuctionString() const override;
	};
}