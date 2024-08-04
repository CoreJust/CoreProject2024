// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstPrinter.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the AstPrinter class that allows to print the AST.
*	It is derived from AstVisitor.
*/

#pragma once
#include "AstVisitor.hpp"
#include "utils/IndentPrinter.hpp"

namespace ast_visitor {
	class AstPrinter final : public AstVisitor</* Self = */AstPrinter, /* ExpressionResult = */void, /* StatementResult = */void> {
	private:
		using Parent = AstVisitor<AstPrinter, void, void>;

		friend class Parent;

	private:
		utils::IndentPrinter& m_printer;

	public:
		AstPrinter(utils::IndentPrinter& printer) noexcept;

		// Prints the given AST to the given IndentPrinter in a human-friendly format.
		void print(utils::NoNull<ast::Node> node);

	protected:
		void visit(ast::LiteralValue& node) override;
		void visit(ast::IdentifierValue& node) override;
		void visit(ast::InvocationOperator& node) override;
		void visit(ast::UnaryOperator& node) override;
		void visit(ast::BinaryOperator& node) override;
		void visit(ast::ReturnOperator& node) override;
		void visit(ast::ExpressionStatement& node) override;
		void visit(ast::ScopeStatement& node) override;
		void visit(ast::VariableDeclaration& node) override;
		void visit(ast::FunctionDeclaration& node) override;
		void visit(ast::ModuleDeclarations& node) override;
	};
}