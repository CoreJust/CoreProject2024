// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	IfElseStatement.hpp by CoreJust
*	Created on 15.08.2024
*	Contains the IfElseStatement class that implements the AST node for if[-else] branchins statement.
*/

#pragma once
#include <vector>
#include "../Expression.hpp"
#include "../Statement.hpp"

namespace ast {
	class IfElseStatement final : public Statement {
	private:
		std::vector<utils::NoNull<Expression>> m_conditions;
		std::vector<utils::NoNull<Statement>> m_ifBodies;
		Statement* m_elseBody;

	public:
		IfElseStatement(std::vector<utils::NoNull<Expression>> conditions, std::vector<utils::NoNull<Statement>> ifBodies, Statement* elseBody) noexcept;
		~IfElseStatement() override;

		std::vector<utils::NoNull<Expression>>& getConditions() noexcept;
		std::vector<utils::NoNull<Statement>>& getIfBodies() noexcept;
		Statement*& getElseBody() noexcept;

		// Returns true if the else branch is present.
		bool hasElse() const noexcept;
	};
}