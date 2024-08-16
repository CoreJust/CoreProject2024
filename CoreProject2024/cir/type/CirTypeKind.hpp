// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirTypeKind.hpp by CoreJust
*	Created on 05.08.2024
*	Contains TypeKind enumeration that represents a kind of type in CIR.
*/

#pragma once

namespace cir {
	enum class TypeKind : unsigned char {
		UNIT = 0,
		I32,
		BOOL,

		BASIC_BLOCK,

		TYPE_KINDS_COUNT
	};
}