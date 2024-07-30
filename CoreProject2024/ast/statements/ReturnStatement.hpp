// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ReturnStatement.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the ReturnStatement class that implements the AST node for return statement in the Core source code.
*/

#pragma once
#include "../Statement.hpp"
#include "../Expression.hpp"

namespace ast {
	class ReturnStatement final : public Statement {
	private:
		Expression* m_expression;

	public:
		ReturnStatement(Expression* expression) noexcept;

		Expression*& getExpression() noexcept;
	};
}