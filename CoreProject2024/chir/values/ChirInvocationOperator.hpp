// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirInvocationOperator.hpp by CoreJust
*	Created on 04.08.2024
*	Contains the InvocationOperator class that implements a CHIR node for an invocation operator (function call).
*/

#pragma once
#include <llvm/ADT/SmallVector.h>
#include "../ChirValue.hpp"

namespace chir {
	class InvocationOperator final : public Value {
	private:
		utils::NoNull<Value> m_callee;
		std::vector<utils::NoNull<Value>> m_arguments;

	public:
		InvocationOperator(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, symbol::Type type) noexcept;

		utils::NoNull<Value>& getCallee() noexcept;
		std::vector<utils::NoNull<Value>>& getArguments() noexcept;
	};
}
