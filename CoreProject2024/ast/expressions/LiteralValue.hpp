// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	LiteralValue.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the LiteralValue class that implements the AST node for a literal value in the Core source code.
*/

#pragma once
#include "utils/IntValue.hpp"
#include "utf/String.hpp"
#include "../Expression.hpp"
#include "ast/type/AstType.hpp"

namespace ast {
	class LiteralValue final : public Expression {
	private:
		utf::StringView m_value;
		utils::NoNull<Type> m_type;

	public:
		LiteralValue(utf::StringView value) noexcept; // Int
		LiteralValue(bool value) noexcept; // Bool
		~LiteralValue() override;

		utils::NoNull<Type> getType() const noexcept;
		utf::StringView getRawValue() const noexcept;
		utils::IntValue parseAsInt() const noexcept;
		bool parseAsBool() const noexcept;
	};
}