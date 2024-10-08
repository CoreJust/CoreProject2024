// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirValueId.hpp by CoreJust
*	Created on 06.08.2024
*	Contains the ValueId type that represents an ID of a CIR Value.
*/

#pragma once

namespace cir {
	using ValueId = unsigned long long int;

	static_assert(sizeof(ValueId) == 8);
}