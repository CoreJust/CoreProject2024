// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirPrinter.hpp by CoreJust
*	Created on 04.08.2024
*	Contains the ChirPrinter class that allows to print the CHIR.
*	It is derived from ChirVisitor.
*/

#pragma once
#include "ChirVisitor.hpp"
#include "utils/IndentPrinter.hpp"

namespace chir_visitor {
	class ChirPrinter final : public ChirVisitor</* Self = */ChirPrinter, /* ValueResult = */void, /* StatementResult = */void, /* DeclarationResult = */void> {
	private:
		using Parent = ChirVisitor<ChirPrinter, void, void, void>;

		friend class Parent;

	private:
		utils::IndentPrinter& m_printer;

	public:
		ChirPrinter(utils::IndentPrinter& printer) noexcept;

		// Prints the given CHIR to the given IndentPrinter in a human-friendly format.
		void print(utils::NoNull<chir::Node> node);

	protected:
		void visit(chir::ConstantValue& node) override;
		void visit(chir::SymbolValue& node) override;
		void visit(chir::InvocationOperator& node) override;
		void visit(chir::UnaryOperator& node) override;
		void visit(chir::BinaryOperator& node) override;
		void visit(chir::ReturnOperator& node) override;
		void visit(chir::ValueStatement& node) override;
		void visit(chir::ScopeStatement& node) override;
		void visit(chir::VariableStatement& node) override;
		void visit(chir::VariableDeclaration& node) override;
		void visit(chir::FunctionDeclaration& node) override;
	};
}
