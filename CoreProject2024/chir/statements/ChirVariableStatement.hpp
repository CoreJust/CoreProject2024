// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirVariableStatement.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the VariableStatement class that implements a CHIR node for a local variable declaration.
*/

#pragma once
#include "symbol/VariableSymbol.hpp"
#include "../ChirValue.hpp"
#include "../ChirStatement.hpp"

namespace chir {
	class VariableStatement final : public Statement {
	private:
		symbol::VariableSymbol& m_variable;
		utils::NoNull<Value> m_initialValue;

	public:
		VariableStatement(symbol::VariableSymbol& variable, utils::NoNull<Value> initialValue) noexcept;
		~VariableStatement() override;

		symbol::VariableSymbol& getVariable() noexcept;
		utils::NoNull<Value>& getInitialValue() noexcept;
	};
}
