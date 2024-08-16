// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ComparativeBinaryOperator.hpp by CoreJust
*	Created on 15.08.2024
*	Contains the ComparativeBinaryOperator class that implements the AST node for a 
*	conditional binary operator in the Core source code.
*	It requires a separate node class because the comparatives can be chained.
*/

#pragma once
#include <vector>
#include "../Expression.hpp"

namespace ast {
	class ComparativeBinaryOperator final : public Expression {
	public:
		// The kind of unary operator used.
		enum ComparativeOperatorType : unsigned char {
			EQUALS = 0,
			NOT_EQUALS,
			LESS_EQUALS,
			GREATER_EQUALS,
			LESS,
			GREATER,

			COMPARATIVE_BINARY_OPERATOR_TYPES_COUNT
		};

	private:
		std::vector<utils::NoNull<Expression>> m_expressions;
		std::vector<ComparativeOperatorType> m_operators;

	public:
		ComparativeBinaryOperator(std::vector<ComparativeOperatorType> operators, std::vector<utils::NoNull<Expression>> expressions) noexcept;

		std::vector<utils::NoNull<Expression>>& getExpressions() noexcept;
		std::vector<ComparativeOperatorType>& getOperators() noexcept;
	};
}