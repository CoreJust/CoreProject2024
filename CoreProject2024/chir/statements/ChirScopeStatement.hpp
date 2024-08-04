// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirScopeStatement.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ScopeStatement class that implements a CHIR node for a scope of statements.
*/

#pragma once
#include <vector>
#include "../ChirStatement.hpp"

namespace chir {
	class ScopeStatement final : public Statement {
	private:
		std::vector<utils::NoNull<Statement>> m_statements;

	public:
		ScopeStatement(std::vector<utils::NoNull<Statement>> statements) noexcept;

		const std::vector<utils::NoNull<Statement>>& getStatements() const noexcept;
	};
}
