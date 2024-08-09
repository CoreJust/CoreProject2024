// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	IdentifierValue.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the IdentifierValue class that implements the AST node for an identifier value in the Core source code.
*/

#pragma once
#include "utf/String.hpp"
#include "../Expression.hpp"

namespace ast {
	class IdentifierValue final : public Expression {
	private:
		utf::StringView m_identifier;

	public:
		IdentifierValue(utf::StringView identifier) noexcept;

		utf::StringView getIdentifier() const noexcept;
	};
}