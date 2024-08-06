// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirTypeKind.hpp by CoreJust
*	Created on 05.08.2024
*	Contains TypeKind enumeration that represents a kind of type in CIR.
*/

#pragma once
#include <cstdint>

namespace cir {
	enum class TypeKind : uint8_t {
		UNIT = 0,
		I32,

		BASIC_BLOCK,

		TYPE_KINDS_COUNT
	};
}