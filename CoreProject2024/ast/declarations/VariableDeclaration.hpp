// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	VariableDeclaration.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the VariableDeclaration class that implements the AST node for a variable declaration (be it local or global) in the Core source code.
*/

#pragma once
#include "utf/String.hpp"
#include "ast/Declaration.hpp"
#include "ast/Expression.hpp"
#include "ast/type/AstType.hpp"

namespace ast {
	class VariableDeclaration final : public Declaration {
	private:
		utf::StringView m_name;
		utils::NoNull<Type> m_type;
		utils::NoNull<Expression> m_initialValue;

	public:
		VariableDeclaration(utf::StringView name, utils::NoNull<Type> type, utils::NoNull<Expression> initialValue) noexcept;
		~VariableDeclaration() override;

		utf::StringView getName() const noexcept;
		utils::NoNull<Type> getVariableType() const noexcept;
		utils::NoNull<Expression>& getInitialValue() noexcept;
	};
}