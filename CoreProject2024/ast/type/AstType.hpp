// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstType.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the Type class used in AST for types.
*	Types are not yet inquired during AST phase, so here a simplified type structure is used.
*/

#pragma once
#include "utils/NoNull.hpp"
#include "utils/PooledAllocator.hpp"
#include "utf/String.hpp"
#include "AstTypeKind.hpp"

namespace symbol {
	class Type;
}

namespace ast {
	class Type {
		template<class T> friend class utils::PooledAllocator;

	protected:
		TypeKind m_typeKind;

	protected:
		Type(TypeKind typeKind) noexcept;

	public:
		static utils::NoNull<Type> make(TypeKind typeKind = TypeKind::NO_TYPE);

	public:
		Type() noexcept; // No type

		// Creates the symbol type from AST type.
		// Can only be called after the types are loaded into symbol table.
		virtual utils::NoNull<symbol::Type> makeSymbolType() const;

		// No type is when the type is unknown (not stated explicitly).
		bool isNoType() const noexcept;

		bool isPrimitiveType() const noexcept;
		bool isComplexType() const noexcept;
		bool isFunctionType() const noexcept;
		bool isCCompatibleType() const noexcept;
		bool isIntegerType() const noexcept;

		TypeKind getKind() const noexcept;
		virtual utf::String toString() const noexcept;
	};
}
