// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	VariableDeclaration.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the VariableDeclaration class that implements the AST node for a variable declaration (be it local or global) in the Core source code.
*/

#pragma once
#include "utf/String.hpp"
#include "../Declaration.hpp"
#include "../Expression.hpp"

namespace ast {
	class VariableDeclaration final : public Declaration {
	private:
		utf::StringView m_name;
		utf::StringView m_type; // empty if no type is specified
		Expression* m_initialValue;

	public:
		VariableDeclaration(utf::StringView name, utf::StringView type, Expression* initialValue) noexcept;

		utf::StringView getName() const noexcept;
		utf::StringView getType() const noexcept;
		Expression*& getInitialValue() noexcept;
	};
}