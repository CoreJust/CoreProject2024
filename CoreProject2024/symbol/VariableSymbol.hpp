// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	VariableSymbol.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the VariableSymbol class that represents a global variable symbol.
*/

#pragma once
#include "Symbol.hpp"

namespace symbol {
	class VariableSymbol final : public Symbol {
	private:
		utils::NoNull<Type> m_type;

	public:
		VariableSymbol(SymbolPath path, utf::String name, utils::NoNull<Type> type);

		utils::NoNull<Type> getType() const noexcept;

		utils::NoNull<Type> getSymbolValueType() const override;

		utf::String toString() const noexcept override;
	};
}