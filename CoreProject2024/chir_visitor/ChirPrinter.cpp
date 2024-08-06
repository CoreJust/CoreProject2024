// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirPrinter.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "chir/ChirClassImplementations.hpp"
#include "chir/ChirModule.hpp"

chir_visitor::ChirPrinter::ChirPrinter(utils::IndentPrinter& printer) noexcept
	: Parent(*this), m_printer(printer) { }

void chir_visitor::ChirPrinter::visitRoot(chir::Module& module) {
	for (utils::NoNull<chir::Declaration> declaration : module.getDeclarations()) {
		Parent::visit(declaration);
	}
}

void chir_visitor::ChirPrinter::visit(chir::ConstantValue& node) {
	m_printer.stream() << std::format("{}({})", node.getValueType().toString(), node.getValue());
}

void chir_visitor::ChirPrinter::visit(chir::SymbolValue& node) {
	switch (node.getSymbolKind()) {
		case symbol::VARIABLE: m_printer.stream() << node.getVariable().toString(); break;
		case symbol::FUNCTION: m_printer.stream() << std::format(
			"fn {}<{}>", 
			node.getFunction().getName(), 
			utils::joinToString(node.getFunction().getArguments(), ", ", "", "", [](utils::NoNull<symbol::VariableSymbol> argument) -> utf::String {
				return argument->getType().toString();
			})
		); break;
	default: unreachable();
	}
}

void chir_visitor::ChirPrinter::visit(chir::InvocationOperator& node) {
	Parent::visit(node.getCallee());
	m_printer.stream() << "(";

	const std::vector<utils::NoNull<chir::Value>>& args = node.getArguments();
	if (!args.empty()) {
		Parent::visit(args[0].get());
	}

	for (size_t i = 1; i < args.size(); i++) {
		m_printer.stream() << ", ";
		Parent::visit(args[i].get());
	}

	m_printer.stream() << ")";
}

void chir_visitor::ChirPrinter::visit(chir::UnaryOperator& node) {
	switch (node.getOperator()) {
		case chir::UnaryOperator::PLUS:  m_printer.stream() << "+("; break;
		case chir::UnaryOperator::MINUS: m_printer.stream() << "-("; break;
	default: unreachable();
	}

	Parent::visit(node.getValue());
	m_printer.stream() << ')';
}

void chir_visitor::ChirPrinter::visit(chir::BinaryOperator& node) {
	m_printer.stream() << '(';
	Parent::visit(node.getLeft());

	switch (node.getOperator()) {
		case chir::BinaryOperator::PLUS:	  m_printer.stream() << ") + ("; break;
		case chir::BinaryOperator::MINUS:	  m_printer.stream() << ") - ("; break;
		case chir::BinaryOperator::MULTIPLY:  m_printer.stream() << ") * ("; break;
		case chir::BinaryOperator::DIVIDE:	  m_printer.stream() << ") / ("; break;
		case chir::BinaryOperator::REMAINDER: m_printer.stream() << ") % ("; break;
	default: unreachable();
	}

	Parent::visit(node.getRight());
	m_printer.stream() << ')';
}

void chir_visitor::ChirPrinter::visit(chir::ReturnOperator& node) {
	m_printer.stream() << "return ";
	if (node.getValue() != nullptr) {
		Parent::visit(node.getValue());
	}
}

void chir_visitor::ChirPrinter::visit(chir::ValueStatement& node) {
	m_printer.printIndent();
	Parent::visit(node.getValue());
	m_printer.stream() << '\n';
}

void chir_visitor::ChirPrinter::visit(chir::ScopeStatement& node) {
	m_printer.stream() << "{\n";
	m_printer.increaseIndent();
	for (utils::NoNull<chir::Statement> statement : node.getStatements()) {
		Parent::visit(statement);
	}

	m_printer.decreaseIndent();
	m_printer.printIndent();
	m_printer.stream() << "}\n\n";
}

void chir_visitor::ChirPrinter::visit(chir::VariableStatement& node) {
	m_printer.printIndent();
	m_printer.stream() << "local " << node.getVariable().toString();

	m_printer.stream() << " := ";
	Parent::visit(node.getInitialValue());
	m_printer.stream() << '\n';
}

void chir_visitor::ChirPrinter::visit(chir::VariableDeclaration& node) {
	m_printer.printIndent();
	m_printer.stream() << "global " << node.getVariable().toString();

	m_printer.stream() << " := ";
	Parent::visit(node.getInitialValue());
	m_printer.stream() << '\n';
}

void chir_visitor::ChirPrinter::visit(chir::FunctionDeclaration& node) {
	m_printer.printIndent();
	m_printer.stream() << node.getFunction().toString();

	m_printer.stream() << ' ';
	if (node.isNative()) {
		m_printer.stream() << "= native(\"" << node.getBodyAsNative() << "\")";
	} else {
		Parent::visit(node.getBodyAsStatement());
	}

	m_printer.stream() << "\n\n";
}
