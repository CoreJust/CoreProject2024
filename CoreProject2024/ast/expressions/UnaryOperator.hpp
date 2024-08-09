// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	UnaryOperator.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the UnaryOperator class that implements the AST node for a common unary operator in the Core source code.
*/

#pragma once
#include "../Expression.hpp"

namespace ast {
	class UnaryOperator final : public Expression {
	public:
		// The kind of unary operator used.
		enum UnaryOperatorType : uint8_t {
			PLUS = 0,
			MINUS,

			UNARY_OPERATOR_TYPES_COUNT
		};

	private:
		utils::NoNull<Expression> m_expression;
		UnaryOperatorType m_operator;

	public:
		UnaryOperator(UnaryOperatorType operatorType, utils::NoNull<Expression> expression) noexcept;

		utils::NoNull<Expression>& getExpression() noexcept;
		UnaryOperatorType getOperator() const noexcept;
	};
}