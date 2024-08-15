// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirConstantValue.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ConstantValue class that implements a CHIR node for a constant (literal) value.
*/

#pragma once
#include "../ChirValue.hpp"

namespace chir {
	class ConstantValue final : public Value {
	private:
		int64_t m_value;

	public:
		ConstantValue(symbol::Type type, int64_t value) noexcept;

		int64_t getValue() const noexcept;
	};
}