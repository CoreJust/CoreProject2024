// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CHIRGenerator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the CHIRGenerator class that implement an AST visitor that generates the
*	Core High-level IR.
*/

#pragma once
#include <vector>
#include "AstVisitor.hpp"
#include "symbol/SymbolTable.hpp"
#include "chir/ChirModule.hpp"
#include "chir/ChirClassDeclarations.hpp"

namespace ast_visitor {
	/*
	*	Visit methods return nullptr for all the declarations.
	*/
	class CHIRGenerator final : public AstVisitor</* Self = */CHIRGenerator, /* ExpressionResult = */utils::NoNull<chir::Value>, /* StatementResult = */chir::Statement*> {
	private:
		using Parent = AstVisitor<CHIRGenerator, utils::NoNull<chir::Value>, chir::Statement*>;

		friend class Parent;

	private:
		std::unique_ptr<symbol::SymbolTable> m_symbols;
		std::vector<utils::NoNull<chir::Declaration>> m_declarations;

		// For function type inquires.
		// It specifies the argument types that we expect the function to have.
		std::vector<utils::NoNull<symbol::Type>> m_typeInquire;
		bool m_hasTypeInquire = false;

	public:
		CHIRGenerator(std::unique_ptr<symbol::SymbolTable> symbols) noexcept;

		std::unique_ptr<chir::Module> generateCHIRModule(utils::NoNull<ast::Declaration> node);

	private:
		utils::NoNull<chir::Value> visit(ast::LiteralValue& node) override;
		utils::NoNull<chir::Value> visit(ast::IdentifierValue& node) override;
		utils::NoNull<chir::Value> visit(ast::InvocationOperator& node) override;
		utils::NoNull<chir::Value> visit(ast::UnaryOperator& node) override;
		utils::NoNull<chir::Value> visit(ast::AsOperator& node) override;
		utils::NoNull<chir::Value> visit(ast::BinaryOperator& node) override;
		utils::NoNull<chir::Value> visit(ast::ComparativeBinaryOperator& node) override;
		utils::NoNull<chir::Value> visit(ast::ReturnOperator& node) override;
		chir::Statement* visit(ast::ExpressionStatement& node) override;
		chir::Statement* visit(ast::ScopeStatement& node) override;
		chir::Statement* visit(ast::IfElseStatement& node) override;
		chir::Statement* visit(ast::VariableDeclaration& node) override;
		chir::Statement* visit(ast::FunctionDeclaration& node) override;
		chir::Statement* visit(ast::ModuleDeclarations& node) override;
	};
}