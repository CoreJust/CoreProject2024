// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	FunctionDeclaration.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the FunctionDeclaration class that implements the AST node for a function declaration (be it local or global) in the Core source code.
*/

#pragma once
#include <variant>
#include <llvm/ADT/SmallVector.h>
#include "utf/String.hpp"
#include "../Declaration.hpp"
#include "../AstType.hpp"

namespace ast {
	class FunctionDeclaration final : public Declaration {
	public:
		struct Argument {
			utf::StringView name;
			Type type;
		};

	private:
		utf::StringView m_name;
		Type m_returnType; // empty if no type is specified
		std::vector<Argument> m_arguments;
		std::variant<utils::NoNull<Statement>, utf::StringView> m_body;

	public:
		FunctionDeclaration(utf::StringView name, Type returnType, std::vector<Argument> arguments, utils::NoNull<Statement> body) noexcept;
		FunctionDeclaration(utf::StringView name, Type returnType, std::vector<Argument> arguments, utf::StringView nativeFunctionName) noexcept;

		utf::StringView getName() const noexcept;
		const Type& getReturnType() const noexcept;
		const std::vector<Argument>& getArguments() const noexcept;

		bool isNative() const noexcept;
		utils::NoNull<Statement>& getBodyAsStatement() noexcept;
		utf::StringView getBodyAsNative() const noexcept;
	};
}