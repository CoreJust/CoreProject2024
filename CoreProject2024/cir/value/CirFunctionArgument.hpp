// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirFunctionArgument.hpp by CoreJust
*	Created on 05.08.2024
*	Contains FunctionArgument class that is derived from Value and represents
*	a function argument in the CIR.
*	It is a Value kind separate from 2 other major categories: Instructions and Constants.
*/

#pragma once
#include "CirValue.hpp"

namespace cir {
	class FunctionArgument final : public Value {
	public:
		FunctionArgument(utf::String name, Type type) noexcept;

		utf::String toString() const;
	};
}