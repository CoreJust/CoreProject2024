// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGotoInstruction.hpp by CoreJust
*	Created on 15.08.2024
*	Contains GotoInstruction class that is derived from Terminator
*	and represents an unconditional jump instruction in CIR.
*/

#pragma once
#include "CirTerminator.hpp"
#include "cir/value/constant/CirBasicBlock.hpp"

namespace cir {
	class GotoInstruction final : public Terminator {
	private:
		utils::NoNull<BasicBlock> m_basicBlockToGo;

	public:
		GotoInstruction(utils::NoNull<BasicBlock> basicBlockToGo) noexcept;

		utils::NoNull<BasicBlock>& getBasicBlockToGo() noexcept;

		utf::String toInstuctionString() const override;
	};
}