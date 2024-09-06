// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirConstant.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Constant class that is derived from Value and is base for all constant 
*	(compile-time known) entities in CIR.
*	It is the literals and addresses within the code.
*/

#pragma once
#include "../CirValue.hpp"

namespace cir {
	class Constant : public Value {
	public:
		Constant(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept;
	};
}