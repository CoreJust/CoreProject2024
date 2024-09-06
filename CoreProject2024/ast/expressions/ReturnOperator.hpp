// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ReturnOperator.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the ReturnOperator class that implements the AST node for return operator in the Core source code.
*/

#pragma once
#include "../Expression.hpp"

namespace ast {
	class ReturnOperator final : public Expression {
	private:
		// Can be null!
		Expression* m_expression;

	public:
		ReturnOperator() noexcept; // When returning from function that has unit return type.
		ReturnOperator(utils::NoNull<Expression> expression) noexcept;
		~ReturnOperator() override;

		Expression*& getExpression() noexcept;
	};
}