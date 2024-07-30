// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstVisitor.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the AstVisitor class that allows to go through all the Nodes in AST.
*	It is base for all the other AST visitors.
*/

#pragma once
#include "utils/Macro.hpp"
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "ast/Declaration.hpp"
#include "ast/AstClassDeclarations.hpp"

namespace ast_visitor {
	/*
	*	To create a visitor, one must inherit their visitor class from this one with Self = the derived class,
	*	which allows non-virtual invocation of the derived class visit methods.
	*	ExpressionResult, StatementResult and DeclarationResult allow to specify the return type of visit functions
	*	for corresponding nodes.
	*/
	template<class Self, class ExpressionResult, class StatementResult, class DeclarationResult>
	class AstVisitor {
	private:
		Self& m_self;

	protected:
		AstVisitor(Self& self) noexcept : m_self(self) { }

	public:
		INLINE void visitRoot(ast::Node* node) {
			if (node->isExpression()) {
				visit(reinterpret_cast<ast::Expression*>(node));
			} else if (node->isDeclaration()) {
				visit(reinterpret_cast<ast::Declaration*>(node));
			} else if (node->isStatement()) {
				visit(reinterpret_cast<ast::Statement*>(node));
			} else {
				unreachable();
			}
		}

		INLINE ExpressionResult visit(ast::Expression* node) {
			switch (node->getType()) {
				case ast::NodeType::LITERAL_VALUE:			return m_self.visit(*reinterpret_cast<ast::LiteralValue*>(node));
				case ast::NodeType::IDENTIFIER_VALUE:		return m_self.visit(*reinterpret_cast<ast::IdentifierValue*>(node));
				case ast::NodeType::INVOCATION_OPERATOR:	return m_self.visit(*reinterpret_cast<ast::InvocationOperator*>(node));
				case ast::NodeType::UNARY_OPERATOR:			return m_self.visit(*reinterpret_cast<ast::UnaryOperator*>(node));
				case ast::NodeType::BINARY_OPERATOR:		return m_self.visit(*reinterpret_cast<ast::BinaryOperator*>(node));
			default: unreachable();
			}
		}

		INLINE StatementResult visit(ast::Statement* node) {
			switch (node->getType()) {
				case ast::NodeType::EXPRESSION_STATEMENT:	return m_self.visit(*reinterpret_cast<ast::ExpressionStatement*>(node));
				case ast::NodeType::SCOPE_STATEMENT:		return m_self.visit(*reinterpret_cast<ast::ScopeStatement*>(node));
				case ast::NodeType::RETURN_STATEMENT:		return m_self.visit(*reinterpret_cast<ast::ReturnStatement*>(node));
			default: unreachable();
			}
		}

		INLINE DeclarationResult visit(ast::Declaration* node) {
			switch (node->getType()) {
				case ast::NodeType::VARIABLE_DECLARATION:	return m_self.visit(*reinterpret_cast<ast::VariableDeclaration*>(node));
				case ast::NodeType::FUNCTION_DECLARATION:	return m_self.visit(*reinterpret_cast<ast::FunctionDeclaration*>(node));
			default: unreachable();
			}
		}

	protected:
		virtual ExpressionResult visit(ast::LiteralValue& node) = 0;
		virtual ExpressionResult visit(ast::IdentifierValue& node) = 0;
		virtual ExpressionResult visit(ast::InvocationOperator& node) = 0;
		virtual ExpressionResult visit(ast::UnaryOperator& node) = 0;
		virtual ExpressionResult visit(ast::BinaryOperator& node) = 0;
		virtual StatementResult visit(ast::ExpressionStatement& node) = 0;
		virtual StatementResult visit(ast::ScopeStatement& node) = 0;
		virtual StatementResult visit(ast::ReturnStatement& node) = 0;
		virtual StatementResult visit(ast::VariableDeclaration& node) = 0;
		virtual StatementResult visit(ast::FunctionDeclaration& node) = 0;
	};
}