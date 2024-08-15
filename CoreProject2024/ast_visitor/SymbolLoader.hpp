// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SymbolLoader.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the SymbolLoader class that traverses the AST and loads the symbols to the symbol table.
*/

#pragma once
#include "AstVisitor.hpp"
#include "symbol/SymbolTable.hpp"

namespace ast_visitor {
	class SymbolLoader final : public AstVisitor</* Self = */SymbolLoader, /* ExpressionResult = */void, /* StatementResult = */void> {
	private:
		using Parent = AstVisitor<SymbolLoader, void, void>;

		friend class Parent;

	private:
		symbol::SymbolTable& m_symbols;

	public:
		SymbolLoader(symbol::SymbolTable& symbols) noexcept;

		// Loads the symbols from the given AST to the symbol table passed through the constructor.
		void loadSymbols(ast::Declaration* node);

	protected:
		void visit(ast::LiteralValue& node) override;
		void visit(ast::IdentifierValue& node) override;
		void visit(ast::InvocationOperator& node) override;
		void visit(ast::UnaryOperator& node) override;
		void visit(ast::BinaryOperator& node) override;
		void visit(ast::ComparativeBinaryOperator& node) override;
		void visit(ast::ReturnOperator& node) override;
		void visit(ast::ExpressionStatement& node) override;
		void visit(ast::ScopeStatement& node) override;
		void visit(ast::VariableDeclaration& node) override;
		void visit(ast::FunctionDeclaration& node) override;
		void visit(ast::ModuleDeclarations& node) override;
	};
}
