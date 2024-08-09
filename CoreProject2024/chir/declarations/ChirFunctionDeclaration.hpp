// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirFunctionDeclaration.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the FunctionDeclaration class that implements a CHIR node for a global/static function declaration.
*/

#pragma once
#include <variant>
#include "symbol/FunctionSymbol.hpp"
#include "../ChirStatement.hpp"
#include "../ChirDeclaration.hpp"

namespace chir {
	class FunctionDeclaration final : public Declaration {
	private:
		const symbol::FunctionSymbol& m_function;
		std::variant<utils::NoNull<Statement>, utf::String> m_body;

	public:
		FunctionDeclaration(const symbol::FunctionSymbol& function, utils::NoNull<Statement> body) noexcept;
		FunctionDeclaration(const symbol::FunctionSymbol& function, utf::String nativeName) noexcept;

		const symbol::FunctionSymbol& getFunction() const noexcept;

		bool isNative() const noexcept;
		utils::NoNull<Statement>& getBodyAsStatement() noexcept;
		utf::StringView getBodyAsNative() const noexcept;
	};
}
