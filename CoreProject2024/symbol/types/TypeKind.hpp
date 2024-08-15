// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TypeKind.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the TypeKind enumeration that represents a node of the Type tree.
*/

#pragma once
#include <cstdint>

namespace symbol {
	// Enumeration of the primitive types and type categories.
	enum class TypeKind : uint8_t {
		UNIT = 0,
		I32,
		BOOL,

		NEVER_TYPE,

		NO_TYPE,

		TYPE_KINDS_COUNT
	};

	constexpr bool isPrimitive(TypeKind type) noexcept {
		return true; // Currently all the basic types are primitive
	}
}