// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirVariableDeclaration.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the VariableDeclaration class that implements a CHIR node for a global/static variable declaration.
*/

#pragma once
#include "symbol/VariableSymbol.hpp"
#include "../ChirValue.hpp"
#include "../ChirDeclaration.hpp"

namespace chir {
	class VariableDeclaration final : public Declaration {
	private:
		const symbol::VariableSymbol& m_variable;
		utils::NoNull<Value> m_initialValue;

	public:
		VariableDeclaration(const symbol::VariableSymbol& variable, utils::NoNull<Value> initialValue) noexcept;

		const symbol::VariableSymbol& getVariable() noexcept;
		utils::NoNull<Value>& getInitialValue() noexcept;
	};
}
