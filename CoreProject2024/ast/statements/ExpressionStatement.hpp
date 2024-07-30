// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ExpressionStatement.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the ExpressionStatement class that implements the AST node for statement that consists of an expression in the Core source code.
*/

#pragma once
#include "../Statement.hpp"
#include "../Expression.hpp"

namespace ast {
	class ExpressionStatement final : public Statement {
	private:
		Expression* m_expression;

	public:
		ExpressionStatement(Expression* expression) noexcept;

		Expression*& getExpression() noexcept;
	};
}