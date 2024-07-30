// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstPrinter.hpp"
#include "ast/AstClassImplementations.hpp"

ast_visitor::AstPrinter::AstPrinter(utils::IndentPrinter printer)
	: Parent(*this), m_printer(std::move(printer)) { }

void ast_visitor::AstPrinter::print(ast::Node* node) {
	Parent::visitRoot(node);
	m_printer.stream() << std::flush;
}

void ast_visitor::AstPrinter::visit(ast::LiteralValue& node) {
	m_printer.stream() << node.getValue();
}

void ast_visitor::AstPrinter::visit(ast::IdentifierValue& node) {
	m_printer.stream() << node.getIdentifier();
}

void ast_visitor::AstPrinter::visit(ast::InvocationOperator& node) {
	Parent::visit(node.getCallee());
	m_printer.stream() << "(";
	
	const std::vector<ast::Expression*>& args = node.getArguments();
	if (!args.empty()) {
		Parent::visit(args[0]);
	}

	for (size_t i = 1; i < args.size(); i++) {
		m_printer.stream() << ", ";
		Parent::visit(args[i]);
	}

	m_printer.stream() << ")";
}

void ast_visitor::AstPrinter::visit(ast::UnaryOperator& node) {
	switch (node.getOperator()) {
		case ast::UnaryOperator::PLUS:  m_printer.stream() << '+'; break;
		case ast::UnaryOperator::MINUS: m_printer.stream() << '-'; break;
	default: unreachable();
	}

	Parent::visit(node.getExpression());
}

void ast_visitor::AstPrinter::visit(ast::BinaryOperator& node) {
	Parent::visit(node.getLeft());

	switch (node.getOperator()) {
		case ast::BinaryOperator::PLUS:		 m_printer.stream() << " + "; break;
		case ast::BinaryOperator::MINUS:	 m_printer.stream() << " - "; break;
		case ast::BinaryOperator::MULTIPLY:  m_printer.stream() << " * "; break;
		case ast::BinaryOperator::DIVIDE:	 m_printer.stream() << " / "; break;
		case ast::BinaryOperator::REMAINDER: m_printer.stream() << " % "; break;
	default: unreachable();
	}

	Parent::visit(node.getRight());
}

void ast_visitor::AstPrinter::visit(ast::ExpressionStatement& node) {
	m_printer.printIndent();
	Parent::visit(node.getExpression());
	m_printer.stream() << '\n';
}

void ast_visitor::AstPrinter::visit(ast::ScopeStatement& node) {
	m_printer.stream() << "{\n";
	m_printer.increaseIndent();
	for (ast::Statement* statement : node.getStatements()) {
		Parent::visit(statement);
	}

	m_printer.decreaseIndent();
	m_printer.printIndent();
	m_printer.stream() << "}\n\n";
}

void ast_visitor::AstPrinter::visit(ast::ReturnStatement& node) {
	m_printer.printIndent();
	m_printer.stream() << "return ";
	Parent::visit(node.getExpression());
	m_printer.stream() << '\n';
}

void ast_visitor::AstPrinter::visit(ast::VariableDeclaration& node) {
	m_printer.printIndent();
	m_printer.stream() << "let " << node.getName();
	if (auto type = node.getType(); !type.empty()) {
		m_printer.stream() << ": " << type;
	}

	m_printer.stream() << " = ";
	Parent::visit(node.getInitialValue());
	m_printer.stream() << '\n';
}

void ast_visitor::AstPrinter::visit(ast::FunctionDeclaration& node) {
	m_printer.printIndent();
	m_printer.stream() << "fn " << node.getName() << '(';

	const std::vector<ast::FunctionDeclaration::Argument>& args = node.getArguments();
	if (!args.empty()) {
		m_printer.stream() << args[0].name << ": " << args[0].type;
	}

	for (size_t i = 1; i < args.size(); i++) {
		m_printer.stream() << ", ";
		m_printer.stream() << args[i].name << ": " << args[i].type;
	}

	m_printer.stream() << ')';
	if (auto returnType = node.getReturnType(); !returnType.empty()) {
		m_printer.stream() << ": " << returnType;
	}

	m_printer.stream() << ' ';
	if (node.isNative()) {
		m_printer.stream() << "= native(\"" << node.getBodyAsNative() << "\")";
	} else {
		Parent::visit(node.getBodyAsStatement());
	}

	m_printer.stream() << "\n\n";
}
