// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TypeKind.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the TypeKind enumeration that represents a node of the Type tree.
*/

#pragma once

namespace ast {
	// Enumeration of the primitive types and type categories.
	enum class TypeKind : unsigned char {
		NO_TYPE = 0,
		UNIT,

		// General integral types
		I8,
		I16,
		I32,
		I64,
		I128,
		ISIZE,
		U8,
		U16,
		U32,
		U64,
		U128,
		USIZE,

		// C-compatible types
		CINT,
		CLONG,
		CUINT,
		CULONG,

		// Literal types
		INT_LITERAL,

		BOOL,

		// Complex types
		FUNCTION,

		TYPE_KINDS_COUNT
	};

	/*
	*	A type is primitive when it doesn't have any subtypes.
	*/
	constexpr bool isPrimitive(TypeKind type) noexcept {
		return type != TypeKind::FUNCTION;
	}

	/*
	*	A type is complex when it has some subtypes.
	*/
	constexpr bool isComplex(TypeKind type) noexcept {
		return type == TypeKind::FUNCTION;
	}

	constexpr bool isCCompatible(TypeKind type) noexcept {
		return type >= TypeKind::CINT && type <= TypeKind::CULONG;
	}

	constexpr bool isInteger(TypeKind type) noexcept {
		return type >= TypeKind::I8 && type <= TypeKind::INT_LITERAL;
	}
}