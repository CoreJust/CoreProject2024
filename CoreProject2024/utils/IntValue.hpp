// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	IntValue.hpp by CoreJust
*	Created on 24.08.2024
*	Contains the int128_t and uint128_t types, as well as a generalized IntValue type.
*/

#pragma once
#include "ap.hpp"

using int128_t = ap_int<128>;
using uint128_t = ap_uint<128>;

namespace utils {
	using IntValue = ap_int<129>;

	template<size_t Size, bool IsSigned>
	ap::library::integer<Size, IsSigned> abs(const ap::library::integer<Size, IsSigned>& value) noexcept {
		return value < 0 ? -value : value;
	}
}