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
		INLINE void visitRoot(utils::NoNull<chir::Node> node) {
			if (node->isValue()) {
				visit(reinterpret_cast<chir::Value*>(node.get()));
			} else if (node->isStatement()) {
				visit(reinterpret_cast<chir::Statement*>(node.get()));
			} else if (node->isDeclaration()) {
				visit(reinterpret_cast<chir::Declaration*>(node.get()));
			} else {
				unreachable();
			}
		}

		INLINE ValueResult visit(utils::NoNull<chir::Value> node) {
			switch (node->getType()) {
				case chir::NodeType::CONSTANT_VALUE:		return m_self.visit(*reinterpret_cast<chir::ConstantValue*>(node.get()));
				case chir::NodeType::SYMBOL_VALUE:			return m_self.visit(*reinterpret_cast<chir::SymbolValue*>(node.get()));
				case chir::NodeType::INVOCATION_OPERATOR:	return m_self.visit(*reinterpret_cast<chir::InvocationOperator*>(node.get()));
				case chir::NodeType::UNARY_OPERATOR:		return m_self.visit(*reinterpret_cast<chir::UnaryOperator*>(node.get()));
				case chir::NodeType::BINARY_OPERATOR:		return m_self.visit(*reinterpret_cast<chir::BinaryOperator*>(node.get()));
				case chir::NodeType::RETURN_OPERATOR:		return m_self.visit(*reinterpret_cast<chir::ReturnOperator*>(node.get()));
			default: unreachable();
			}
		}

		INLINE StatementResult visit(utils::NoNull<chir::Statement> node) {
			switch (node->getType()) {
				case chir::NodeType::VALUE_STATEMENT:		return m_self.visit(*reinterpret_cast<chir::ValueStatement*>(node.get()));
				case chir::NodeType::SCOPE_STATEMENT:		return m_self.visit(*reinterpret_cast<chir::ScopeStatement*>(node.get()));
				case chir::NodeType::VARIABLE_STATEMENT:	return m_self.visit(*reinterpret_cast<chir::VariableStatement*>(node.get()));
			default: unreachable();
			}
		}

		INLINE DeclarationResult visit(utils::NoNull<chir::Declaration> node) {
			switch (node->getType()) {
				case chir::NodeType::VARIABLE_DECLARATION:	return m_self.visit(*reinterpret_cast<chir::VariableDeclaration*>(node.get()));
				case chir::NodeType::FUNCTION_DECLARATION:	return m_self.visit(*reinterpret_cast<chir::FunctionDeclaration*>(node.get()));
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