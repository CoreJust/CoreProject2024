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
	*	ExpressionResult and StatementResult allow to specify the return type of visit functions
	*	for corresponding nodes.
	*/
	template<class Self, class ExpressionResult, class StatementResult>
	class AstVisitor {
	private:
		Self& m_self;

	protected:
		AstVisitor(Self& self) noexcept : m_self(self) { }

	public:
		INLINE void visitRoot(utils::NoNull<ast::Node> node) {
			if (node->isExpression()) {
				visit(node.as<ast::Expression>().get());
			} else if (node->isStatement()) {
				visit(node.as<ast::Declaration>().get());
			} else {
				unreachable();
			}
		}

		INLINE ExpressionResult visit(utils::NoNull<ast::Expression> node) {
			switch (node->getType()) {
				case ast::NodeKind::LITERAL_VALUE:			return m_self.visit(*node.as<ast::LiteralValue>());
				case ast::NodeKind::IDENTIFIER_VALUE:		return m_self.visit(*node.as<ast::IdentifierValue>());
				case ast::NodeKind::INVOCATION_OPERATOR:	return m_self.visit(*node.as<ast::InvocationOperator>());
				case ast::NodeKind::UNARY_OPERATOR:			return m_self.visit(*node.as<ast::UnaryOperator>());
				case ast::NodeKind::BINARY_OPERATOR:		return m_self.visit(*node.as<ast::BinaryOperator>());
				case ast::NodeKind::RETURN_OPERATOR:		return m_self.visit(*node.as<ast::ReturnOperator>());
			default: unreachable();
			}
		}

		INLINE StatementResult visit(utils::NoNull<ast::Statement> node) {
			switch (node->getType()) {
				case ast::NodeKind::EXPRESSION_STATEMENT:	return m_self.visit(*node.as<ast::ExpressionStatement>());
				case ast::NodeKind::SCOPE_STATEMENT:		return m_self.visit(*node.as<ast::ScopeStatement>());
				case ast::NodeKind::VARIABLE_DECLARATION:	return m_self.visit(*node.as<ast::VariableDeclaration>());
				case ast::NodeKind::FUNCTION_DECLARATION:	return m_self.visit(*node.as<ast::FunctionDeclaration>());
				case ast::NodeKind::MODULE_DECLARATIONS:	return m_self.visit(*node.as<ast::ModuleDeclarations>());
			default: unreachable();
			}
		}

	protected:
		virtual ExpressionResult visit(ast::LiteralValue& node) = 0;
		virtual ExpressionResult visit(ast::IdentifierValue& node) = 0;
		virtual ExpressionResult visit(ast::InvocationOperator& node) = 0;
		virtual ExpressionResult visit(ast::UnaryOperator& node) = 0;
		virtual ExpressionResult visit(ast::BinaryOperator& node) = 0;
		virtual ExpressionResult visit(ast::ReturnOperator& node) = 0;
		virtual StatementResult visit(ast::ExpressionStatement& node) = 0;
		virtual StatementResult visit(ast::ScopeStatement& node) = 0;
		virtual StatementResult visit(ast::VariableDeclaration& node) = 0;
		virtual StatementResult visit(ast::FunctionDeclaration& node) = 0;
		virtual StatementResult visit(ast::ModuleDeclarations& node) = 0;
	};
}