// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TypeKind.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the TypeKind enumeration that represents a node of the Type tree.
*/

#pragma once

namespace symbol {
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

		NEVER_TYPE,

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

	/*
	*	Literal types are temporary types used for literals.
	*	These types must be later replaced by the inquired types.
	*/
	constexpr bool isLiteral(TypeKind type) noexcept {
		return type == TypeKind::INT_LITERAL;
	}

	constexpr bool isCCompatible(TypeKind type) noexcept {
		return type >= TypeKind::CINT && type <= TypeKind::CULONG;
	}

	constexpr bool isInteger(TypeKind type) noexcept {
		return type >= TypeKind::I8 && type <= TypeKind::INT_LITERAL;
	}

	constexpr bool isSigned(TypeKind type) noexcept {
		switch (type) {
			case symbol::TypeKind::I8: [[fallthrough]];
			case symbol::TypeKind::I16: [[fallthrough]];
			case symbol::TypeKind::I32: [[fallthrough]];
			case symbol::TypeKind::I64: [[fallthrough]];
			case symbol::TypeKind::I128: [[fallthrough]];
			case symbol::TypeKind::ISIZE: [[fallthrough]];
			case symbol::TypeKind::CINT: [[fallthrough]];
			case symbol::TypeKind::CLONG: return true;
		default: return false;
		}
	}

	constexpr bool isArithmetic(TypeKind type) noexcept {
		return type >= TypeKind::I8 && type <= TypeKind::INT_LITERAL;
	}
}