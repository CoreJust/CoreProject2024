// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirVisitor.hpp by CoreJust
*	Created on 04.08.2024
*	Contains the ChirVisitor class that allows to traverse all the Nodes in CHIR.
*	It is base for all the other CHIR visitors.
*/

#pragma once
#include "utils/Macro.hpp"
#include "chir/ChirValue.hpp"
#include "chir/ChirStatement.hpp"
#include "chir/ChirDeclaration.hpp"
#include "chir/ChirClassDeclarations.hpp"

namespace chir {
	class Module;
}

namespace chir_visitor {
	/*
	*	To create a visitor, one must inherit their visitor class from this one with Self = the derived class,
	*	which allows non-virtual invocation of the derived class visit methods.
	*	ValueResult, StatementResult and DeclarationResult allow to specify the return type of visit functions
	*	for corresponding nodes.
	*/
	template<class Self, class ValueResult, class StatementResult, class DeclarationResult>
	class ChirVisitor {
	private:
		Self& m_self;

	protected:
		ChirVisitor(Self& self) noexcept : m_self(self) { }

	public:
		virtual void visitRoot(chir::Module& module) = 0;

		INLINE ValueResult visit(utils::NoNull<chir::Value> node) {
			switch (node->getKind()) {
				case chir::NodeKind::CONSTANT_VALUE:		return m_self.visit(*node.as<chir::ConstantValue>());
				case chir::NodeKind::SYMBOL_VALUE:			return m_self.visit(*node.as<chir::SymbolValue>());
				case chir::NodeKind::INVOCATION_OPERATOR:	return m_self.visit(*node.as<chir::InvocationOperator>());
				case chir::NodeKind::UNARY_OPERATOR:		return m_self.visit(*node.as<chir::UnaryOperator>());
				case chir::NodeKind::BINARY_OPERATOR:		return m_self.visit(*node.as<chir::BinaryOperator>());
				case chir::NodeKind::RETURN_OPERATOR:		return m_self.visit(*node.as<chir::ReturnOperator>());
			default: unreachable();
			}
		}

		INLINE StatementResult visit(utils::NoNull<chir::Statement> node) {
			switch (node->getKind()) {
				case chir::NodeKind::VALUE_STATEMENT:		return m_self.visit(*node.as<chir::ValueStatement>());
				case chir::NodeKind::SCOPE_STATEMENT:		return m_self.visit(*node.as<chir::ScopeStatement>());
				case chir::NodeKind::VARIABLE_STATEMENT:	return m_self.visit(*node.as<chir::VariableStatement>());
			default: unreachable();
			}
		}

		INLINE DeclarationResult visit(utils::NoNull<chir::Declaration> node) {
			switch (node->getKind()) {
				case chir::NodeKind::VARIABLE_DECLARATION:	return m_self.visit(*node.as<chir::VariableDeclaration>());
				case chir::NodeKind::FUNCTION_DECLARATION:	return m_self.visit(*node.as<chir::FunctionDeclaration>());
			default: unreachable();
			}
		}

	protected:
		virtual ValueResult visit(chir::ConstantValue& node) = 0;
		virtual ValueResult visit(chir::SymbolValue & node) = 0;
		virtual ValueResult visit(chir::InvocationOperator& node) = 0;
		virtual ValueResult visit(chir::UnaryOperator& node) = 0;
		virtual ValueResult visit(chir::BinaryOperator& node) = 0;
		virtual ValueResult visit(chir::ReturnOperator& node) = 0;
		virtual StatementResult visit(chir::ValueStatement & node) = 0;
		virtual StatementResult visit(chir::ScopeStatement& node) = 0;
		virtual StatementResult visit(chir::VariableStatement& node) = 0;
		virtual DeclarationResult visit(chir::VariableDeclaration& node) = 0;
		virtual DeclarationResult visit(chir::FunctionDeclaration& node) = 0;
	};
}