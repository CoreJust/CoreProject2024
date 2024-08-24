// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Type.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the Type class that represents a type within the Core symbol table and CIR.
*	No type is used when the type is not yet known and must be inquired from context.
*/

#pragma once
#include "utils/NoNull.hpp"
#include "utils/PooledAllocator.hpp"
#include "utf/String.hpp"
#include "TypeKind.hpp"

namespace cir {
	class Type;
}

namespace symbol {
	/*
	*	The symbol::Type is used in Symbol Table and in the Core High-level IR.
	* 
	*	A type is represented as a type tree structure with leaves being the
	*	primitive types / references to the symbol table.
	*/
	class Type {
		template<class T> friend class utils::PooledAllocator;

	protected:
		TypeKind m_typeKind;

	protected:
		Type(TypeKind typeKind) noexcept;

	public:
		static utils::NoNull<Type> make(TypeKind typeKind);

		// Creates the CIR type from CHIR type.
		virtual utils::NoNull<cir::Type> makeCirType() const;

		virtual bool equals(const Type& other) const noexcept;

		TypeKind getTypeKind() const noexcept;

		// Returns the size of the type in bytes.
		uint32_t getTypeSize() const noexcept;

		// Returns the string representation of the type as if in the code.
		virtual utf::String toString() const;

		// Returns the string representation of the type that is used for name mangling purposes.
		virtual utf::String toMangleString() const;

		// Returns true if the basic type is NO_TYPE
		bool isNoType() const noexcept;

		bool isNeverType() const noexcept;
		bool isPrimitiveType() const noexcept;
		bool isComplexType() const noexcept;
		bool isLiteralType() const noexcept;
		bool isFunctionType() const noexcept;
		bool isBoolType() const noexcept;
		bool isCCompatibleType() const noexcept;
		bool isIntegerType() const noexcept;
		bool isSignedType() const noexcept;
		bool isArithmeticType() const noexcept;
	};
}