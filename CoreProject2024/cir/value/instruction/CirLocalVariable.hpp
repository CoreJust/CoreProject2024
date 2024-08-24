// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirLocalVariable.hpp by CoreJust
*	Created on 05.08.2024
*	Contains LocalVariable class that is derived from Instruction
*	and represents a local variable declaration in CIR.
*/

#pragma once
#include "CirInstruction.hpp"

namespace cir {
	class LocalVariable final : public Instruction {
	private:
		utils::NoNull<Value> m_initialValue;

	public:
		LocalVariable(utf::String name, utils::NoNull<Type> type, utils::NoNull<Value> initialValue) noexcept;

		utils::NoNull<Value>& getInitialValue() noexcept;

		utf::String toInstuctionString() const override;
	};
}