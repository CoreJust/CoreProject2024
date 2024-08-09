// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirVariableValue.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the VariableValue class that implements a CHIR node for a symbol (variable or function) use value.
*/

#pragma once
#include "../ChirValue.hpp"
#include "symbol/FunctionSymbol.hpp"

namespace chir {
	class SymbolValue final : public Value {
	private:
		utils::NoNull<symbol::Symbol> m_symbol;

	public:
		SymbolValue(utils::NoNull<symbol::Symbol> symbol) noexcept;

		symbol::SymbolKind getSymbolKind() const noexcept;
		const symbol::Symbol& getSymbol() const noexcept;
		const symbol::FunctionSymbol& getFunction() const noexcept;
		const symbol::VariableSymbol& getVariable() const noexcept;
	};
}
