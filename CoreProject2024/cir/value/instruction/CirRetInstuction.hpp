// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirRetInstruction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains RetInstruction class that is derived from Terminator
*	and represents a return instruction in CIR.
*/

#pragma once
#include "CirTerminator.hpp"

namespace cir {
	class RetInstruction final : public Terminator {
	private:
		Value* m_operand;

	public:
		RetInstruction() noexcept;
		RetInstruction(utils::NoNull<Value> operand) noexcept;

		bool isRetUnit() const noexcept;
		Value*& getOperand() noexcept;
		const Type& getReturnType() noexcept;

		utf::String toInstuctionString() const override;
	};
}