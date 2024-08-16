// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Parser.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the Parser class that allows to parse a token stream into AST.
*/

#pragma once
#include <vector>
#include "lexer/Tokenizer.hpp"
#include "ast/Declaration.hpp"
#include "ast/Statement.hpp"
#include "ast/Expression.hpp"
#include "ast/AstType.hpp"

namespace parser {
	class Parser final {
	private:
		lexer::Tokenizer m_toks;

	public:
		Parser(lexer::Tokenizer tokenizer);

		ast::Declaration* parse();

	private:
		// Declarations
		ast::Declaration* functionDeclaration();
		ast::Declaration* variableDeclaration();

		// Statements
		ast::Statement* statement();
		ast::Statement* scopeStatement();
		ast::Statement* ifElseStatement();

		// Expressions
		ast::Expression* expression();
		ast::Expression* returnOperator();
		ast::Expression* logical();
		ast::Expression* comparative();
		ast::Expression* additive();
		ast::Expression* multiplicative();
		ast::Expression* unary();
		ast::Expression* postprimary();
		ast::Expression* primary();

		// Types
		ast::Type parseType();
	};
}
