// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirConstantNumber.hpp by CoreJust
*	Created on 05.08.2024
*	Contains ConstantNumber class that is derived from Constant and 
*	represents a constant number (a literal) in CIR code.
*/

#pragma once
#include "CirConstant.hpp"

namespace cir {
	/*
	*	Since any Value has a name and a number doesn't need one,
	*	the value is stored within the name bits.
	*/
	class ConstantNumber final : public Constant {
	public:
		ConstantNumber(Type type, int64_t value) noexcept;

		int64_t getValue() const noexcept;
	};
}