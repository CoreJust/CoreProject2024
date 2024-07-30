// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	LiteralValue.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the LiteralValue class that implements the AST node for a literal value in the Core source code.
*/

#pragma once
#include "utf/String.hpp"
#include "../Expression.hpp"

namespace ast {
	class LiteralValue final : public Expression {
	private:
		utf::StringView m_value;
		// <ValueType> m_type;

	public:
		LiteralValue(utf::StringView value) noexcept;

		utf::StringView getValue() const noexcept;
	};
}