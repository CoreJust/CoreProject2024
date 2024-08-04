// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Type.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the Type class that represents a type within the Core symbol table and CIR.
*/

#pragma once
#include "utf/String.hpp"
#include "BasicType.hpp"

namespace symbol {
	/*
	*	The symbol::Type is used in Symbol Table and in the Core High-level IR.
	* 
	*	A type is represented as a type tree structure with leaves being the
	*	primitive types / references to the symbol table.
	*/
	class Type {
	private:
		BasicType m_basicType;

	public:
		Type(BasicType basicType) noexcept;

		bool operator==(const Type& other) const noexcept;

		BasicType getBasicType() const noexcept;

		// Returns the size of the type in bytes.
		uint32_t getTypeSize() const noexcept;

		// Returns the string representation of the type as if in the code.
		utf::String toString() const;

		// Returns the string representation of the type that is used for name mangling purposes.
		utf::String toMangleString() const;

		// Returns true if the basic type is NO_TYPE
		bool isNoType() const noexcept;
	};
}