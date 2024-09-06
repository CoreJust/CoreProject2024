// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ScopeStatement.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the ScopeStatement class that implements the AST node for a scope of statements in the Core source code.
*/

#pragma once
#include <vector>
#include "../Statement.hpp"

namespace ast {
	class ScopeStatement final : public Statement {
	private:
		std::vector<utils::NoNull<Statement>> m_statements;

	public:
		ScopeStatement(std::vector<utils::NoNull<Statement>> statements) noexcept;
		~ScopeStatement() override;

		const std::vector<utils::NoNull<Statement>>& getStatements() noexcept;
	};
}