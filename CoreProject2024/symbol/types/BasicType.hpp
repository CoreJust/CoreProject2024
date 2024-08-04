// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	BasicType.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the BasicType enumeration that represents a node of the Type tree.
*/

#pragma once
#include <cstdint>

namespace symbol {
	// Enumeration of the primitive types and type categories.
	enum class BasicType : uint8_t {
		UNIT = 0,
		I32,

		NEVER_TYPE,

		NO_TYPE
	};

	constexpr bool isPrimitive(BasicType type) noexcept {
		return true; // Currently all the basic types are primitive
	}
}