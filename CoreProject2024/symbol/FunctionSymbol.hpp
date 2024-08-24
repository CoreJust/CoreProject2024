// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	FunctionSymbol.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the FunctionSymbol class that represents a global function symbol.
*/

#pragma once
#include <vector>
#include "utils/NoNull.hpp"
#include "VariableSymbol.hpp"

namespace symbol {
	class FunctionSymbol final : public Symbol {
	private:
		utils::NoNull<Type> m_returnType;
		std::vector<utils::NoNull<VariableSymbol>> m_arguments;

	public:
		FunctionSymbol(SymbolPath path, utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<VariableSymbol>> arguments);

		utf::String makeMangledName() const;

		utils::NoNull<Type> getReturnType() const noexcept;
		const std::vector<utils::NoNull<VariableSymbol>>& getArguments() const noexcept;

		utils::NoNull<Type> getSymbolValueType() const override;

		utf::String toString() const noexcept override;
	};
}