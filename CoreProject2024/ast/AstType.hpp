// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstType.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the Type class used in AST for types.
*	Types are not yet inquired during AST phase, so here a simplified type structure is used.
*/

#pragma once
#include "utf/String.hpp"

namespace symbol {
	class Type;
}

namespace ast {
	class Type final {
	private:
		utf::StringView m_typeName;

	public:
		Type() noexcept; // No type
		Type(utf::StringView typeName) noexcept;

		// Creates the symbol type from AST type.
		// Can only be called after the types are loaded into symbol table.
		symbol::Type makeSymbolType() const;

		// No type is when the type is unknown (not stated explicitly).
		bool isNoType() const noexcept;

		utf::StringView getTypeName() const noexcept;
	};
}
