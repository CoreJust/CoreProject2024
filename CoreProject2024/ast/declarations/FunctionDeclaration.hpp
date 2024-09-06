// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	FunctionDeclaration.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the FunctionDeclaration class that implements the AST node for a function declaration (be it local or global) in the Core source code.
*/

#pragma once
#include <variant>
#include <vector>
#include "utf/String.hpp"
#include "ast/Declaration.hpp"
#include "ast/type/AstType.hpp"

namespace ast {
	class FunctionDeclaration final : public Declaration {
	public:
		struct Argument {
			utf::StringView name;
			utils::NoNull<Type> type;
		};

	private:
		utf::StringView m_name;
		utils::NoNull<Type> m_returnType;
		std::vector<Argument> m_arguments;
		std::variant<utils::NoNull<Statement>, utf::StringView> m_body;

	public:
		FunctionDeclaration(utf::StringView name, utils::NoNull<Type> returnType, std::vector<Argument> arguments, utils::NoNull<Statement> body) noexcept;
		FunctionDeclaration(utf::StringView name, utils::NoNull<Type> returnType, std::vector<Argument> arguments, utf::StringView nativeFunctionName) noexcept;
		~FunctionDeclaration();

		utf::StringView getName() const noexcept;
		utils::NoNull<Type> getReturnType() const noexcept;
		const std::vector<Argument>& getArguments() const noexcept;

		bool isNative() const noexcept;
		utils::NoNull<Statement>& getBodyAsStatement() noexcept;
		utf::StringView getBodyAsNative() const noexcept;
	};
}