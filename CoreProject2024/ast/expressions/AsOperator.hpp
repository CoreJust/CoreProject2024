// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AsOperator.hpp by CoreJust
*	Created on 01.09.2024
*	Contains the AsOperator class that implements the AST node for a type conversion (as) operator in the Core source code.
*/

#pragma once
#include "../Expression.hpp"
#include "../type/AstType.hpp"

namespace ast {
	class AsOperator final : public Expression {
	private:
		utils::NoNull<Expression> m_value;
		utils::NoNull<Type> m_type;

	public:
		AsOperator(utils::NoNull<Expression> value, utils::NoNull<Type> type) noexcept;

		utils::NoNull<Expression>& getValue() noexcept;
		utils::NoNull<Type>& getType() noexcept;
	};
}