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
#include "../Declaration.hpp"

namespace ast {
	class FunctionDeclaration final : public Declaration {
	public:
		struct Argument {
			utf::StringView name;
			utf::StringView type;
		};

	private:
		utf::StringView m_name;
		utf::StringView m_returnType; // empty if no type is specified
		std::vector<Argument> m_arguments;
		std::variant<Statement*, utf::StringView> m_body;

	public:
		FunctionDeclaration(utf::StringView name, utf::StringView returnType, std::vector<Argument> arguments, Statement* body) noexcept;
		FunctionDeclaration(utf::StringView name, utf::StringView returnType, std::vector<Argument> arguments, utf::StringView nativeFunctionName) noexcept;

		utf::StringView getName() const noexcept;
		utf::StringView getReturnType() const noexcept;
		const std::vector<Argument>& getArguments() const noexcept;

		bool isNative() const noexcept;
		Statement*& getBodyAsStatement() noexcept;
		utf::StringView getBodyAsNative() const noexcept;
	};
}