// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	VariableSymbol.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the VariableSymbol class that represents a global variable symbol.
*/

#pragma once
#include "Symbol.hpp"
#include "types/Type.hpp"

namespace symbol {
	class VariableSymbol final : public Symbol {
	private:
		symbol::Type m_type;

	public:
		VariableSymbol(SymbolPath path, utf::String name, symbol::Type type);

		const symbol::Type& getType() const noexcept;

		utf::String toString() const noexcept override;
	};
}