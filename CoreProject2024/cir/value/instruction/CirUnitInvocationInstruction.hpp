// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirUnitInvocationInstruction.hpp by CoreJust
*	Created on 05.08.2024
*	Contains InvocationInstruction class that is derived from Instruction
*	and represents a non-unit function call in CIR.
*/

#pragma once
#include "CirInstruction.hpp"

namespace cir {
	class UnitInvocationInstruction final : public Instruction {
	private:
		utils::NoNull<Value> m_callee;
		std::vector<utils::NoNull<Value>> m_arguments;

	public:
		UnitInvocationInstruction(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments) noexcept;
		~UnitInvocationInstruction() override;

		utils::NoNull<Value>& getCallee() noexcept;
		std::vector<utils::NoNull<Value>>& getArguments() noexcept;

		utf::String toInstuctionString() const override;
	};
}