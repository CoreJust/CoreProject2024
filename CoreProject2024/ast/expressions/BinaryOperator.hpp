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
		enum BinaryOperatorType : unsigned char {
			PLUS = 0,
			MINUS,
			MULTIPLY,
			DIVIDE,
			REMAINDER,

			LOGIC_AND,
			LOGIC_OR,

			BINARY_OPERATOR_TYPES_COUNT
		};

	private:
		utils::NoNull<Expression> m_left;
		utils::NoNull<Expression> m_right;
		BinaryOperatorType m_operator;

	public:
		BinaryOperator(BinaryOperatorType operatorType, utils::NoNull<Expression> left, utils::NoNull<Expression> right) noexcept;
		~BinaryOperator() override;

		utils::NoNull<Expression>& getLeft() noexcept;
		utils::NoNull<Expression>& getRight() noexcept;
		BinaryOperatorType getOperator() const noexcept;

		// Returns whether the operator is arithmetical.
		bool isArithmetical() const noexcept;

		// Returns whether the operator is logical.
		bool isLogical() const noexcept;
	};
}