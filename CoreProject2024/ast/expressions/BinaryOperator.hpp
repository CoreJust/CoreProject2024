// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	BinaryOperator.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the BinaryOperator class that implements the AST node for a common binary operator in the Core source code.
*/

#pragma once
#include "../Expression.hpp"

namespace ast {
	class BinaryOperator final : public Expression {
	public:
		// The kind of unary operator used.
		enum BinaryOperatorType : uint8_t {
			PLUS = 0,
			MINUS,
			MULTIPLY,
			DIVIDE,
			REMAINDER,

			UNARY_OPERATOR_TYPES_COUNT
		};

	private:
		Expression* m_left;
		Expression* m_right;
		BinaryOperatorType m_operator;

	public:
		BinaryOperator(BinaryOperatorType operatorType, Expression* left, Expression* right) noexcept;

		Expression*& getLeft() noexcept;
		Expression*& getRight() noexcept;
		BinaryOperatorType getOperator() const noexcept;
	};
}