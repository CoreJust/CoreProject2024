// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstPrinter.hpp"
#include "ast/AstClassImplementations.hpp"

ast_visitor::AstPrinter::AstPrinter(utils::IndentPrinter& printer) noexcept
	: Parent(*this), m_printer(printer) { }

void ast_visitor::AstPrinter::print(utils::NoNull<ast::Node> node) {
	Parent::visitRoot(node);
}

void ast_visitor::AstPrinter::visit(ast::LiteralValue& node) {
	if (node.getType()->getKind() == ast::TypeKind::BOOL) {
		m_printer.stream() << (node.parseAsBool() ? "true\0" : "false");
	} else { // Number
		m_printer.stream() << node.getRawValue();
	}
}

void ast_visitor::AstPrinter::visit(ast::IdentifierValue& node) {
	m_printer.stream() << node.getIdentifier();
}

void ast_visitor::AstPrinter::visit(ast::InvocationOperator& node) {
	if (node.getCallee()->getKind() != ast::NodeKind::IDENTIFIER_VALUE) {
		m_printer.stream() << '(';
		Parent::visit(node.getCallee());
		m_printer.stream() << ')';
	} else {
		Parent::visit(node.getCallee());
	}

	m_printer.stream() << '(';
	
	const std::vector<utils::NoNull<ast::Expression>>& args = node.getArguments();
	if (!args.empty()) {
		Parent::visit(args[0].get());
	}

	for (size_t i = 1; i < args.size(); i++) {
		m_printer.stream() << ", ";
		Parent::visit(args[i].get());
	}

	m_printer.stream() << ")";
}

void ast_visitor::AstPrinter::visit(ast::UnaryOperator& node) {
	switch (node.getOperator()) {
		case ast::UnaryOperator::PLUS:		m_printer.stream() << '+'; break;
		case ast::UnaryOperator::MINUS:		m_printer.stream() << '-'; break;
		case ast::UnaryOperator::LOGIC_NOT: m_printer.stream() << '!'; break;
	default: unreachable();
	}

	if (node.getExpression()->getKind() <= ast::NodeKind::UNARY_OPERATOR) {
		Parent::visit(node.getExpression()); // Priority is obvious, no need for parens.
	} else {
		m_printer.stream() << '(';
		Parent::visit(node.getExpression());
		m_printer.stream() << ')';
	}
}

void ast_visitor::AstPrinter::visit(ast::AsOperator& node) {
	m_printer.stream() << '(';
	Parent::visit(node.getValue());
	m_printer.stream() << ") as " << node.getType()->toString();
}

void ast_visitor::AstPrinter::visit(ast::BinaryOperator& node) {
	if (node.getLeft()->getKind() < ast::NodeKind::BINARY_OPERATOR) {
		Parent::visit(node.getLeft()); // Priority is obvious, no need for parens.
	} else {
		m_printer.stream() << '(';
		Parent::visit(node.getLeft());
		m_printer.stream() << ')';
	}

	switch (node.getOperator()) {
		case ast::BinaryOperator::PLUS:			m_printer.stream() << " + "; break;
		case ast::BinaryOperator::MINUS:		m_printer.stream() << " - "; break;
		case ast::BinaryOperator::MULTIPLY:		m_printer.stream() << " * "; break;
		case ast::BinaryOperator::DIVIDE:		m_printer.stream() << " / "; break;
		case ast::BinaryOperator::REMAINDER:	m_printer.stream() << " % "; break;
		case ast::BinaryOperator::LOGIC_AND:	m_printer.stream() << " && "; break;
		case ast::BinaryOperator::LOGIC_OR:		m_printer.stream() << " || "; break;
	default: unreachable();
	}

	if (node.getRight()->getKind() < ast::NodeKind::BINARY_OPERATOR) {
		Parent::visit(node.getRight()); // Priority is obvious, no need for parens.
	} else {
		m_printer.stream() << '(';
		Parent::visit(node.getRight());
		m_printer.stream() << ')';
	}
}

void ast_visitor::AstPrinter::visit(ast::ComparativeBinaryOperator& node) {
	const std::vector<ast::ComparativeBinaryOperator::ComparativeOperatorType>& operators = node.getOperators();
	const std::vector<utils::NoNull<ast::Expression>>& expressions = node.getExpressions();

	for (uint32_t i = 0; i < expressions.size(); ++i) {
		if (expressions[i]->getKind() < ast::NodeKind::BINARY_OPERATOR) {
			Parent::visit(expressions[i]); // Priority is obvious, no need for parens.
		} else {
			m_printer.stream() << '(';
			Parent::visit(expressions[i]);
			m_printer.stream() << ')';
		}

		if (i < operators.size()) {
			switch (operators[i]) {
				case ast::ComparativeBinaryOperator::EQUALS:		m_printer.stream() << " == "; break;
				case ast::ComparativeBinaryOperator::NOT_EQUALS:	m_printer.stream() << " != "; break;
				case ast::ComparativeBinaryOperator::LESS_EQUALS:	m_printer.stream() << " <= "; break;
				case ast::ComparativeBinaryOperator::GREATER_EQUALS: m_printer.stream() << " >= "; break;
				case ast::ComparativeBinaryOperator::LESS:			m_printer.stream() << " < "; break;
				case ast::ComparativeBinaryOperator::GREATER:		m_printer.stream() << " > "; break;
			default: unreachable();
			}
		}
	}
}

void ast_visitor::AstPrinter::visit(ast::ReturnOperator& node) {
	m_printer.stream() << "return ";
	if (node.getExpression() != nullptr) {
		Parent::visit(node.getExpression());
	}
}

void ast_visitor::AstPrinter::visit(ast::ExpressionStatement& node) {
	m_printer.printIndent();
	Parent::visit(node.getExpression());
	m_printer.stream() << '\n';
}

void ast_visitor::AstPrinter::visit(ast::ScopeStatement& node) {
	m_printer.stream() << "{\n";
	m_printer.increaseIndent();
	for (utils::NoNull<ast::Statement> statement : node.getStatements()) {
		Parent::visit(statement);
	}

	m_printer.decreaseIndent();
	m_printer.printIndent();
	m_printer.stream() << "}";
}

void ast_visitor::AstPrinter::visit(ast::IfElseStatement& node) {
	const std::vector<utils::NoNull<ast::Expression>>& conditions = node.getConditions();
	const std::vector<utils::NoNull<ast::Statement>>& ifBodies = node.getIfBodies();

	m_printer.printIndent();
	m_printer.stream() << "if (";
	Parent::visit(conditions[0]);
	m_printer.stream() << ") ";

	Parent::visit(ifBodies[0]);

	for (uint32_t i = 1; i < conditions.size(); ++i) {
		m_printer.stream() << " elif (";
		Parent::visit(conditions[i]);
		m_printer.stream() << ") ";

		Parent::visit(ifBodies[i]);
	}

	if (node.hasElse()) {
		m_printer.stream() << " else ";
		Parent::visit(node.getElseBody());
	}

	m_printer.stream() << "\n\n";
}

void ast_visitor::AstPrinter::visit(ast::VariableDeclaration& node) {
	m_printer.printIndent();
	m_printer.stream() << "let " << node.getName();
	if (auto type = node.getVariableType(); !type->isNoType()) {
		m_printer.stream() << ": " << type->toString();
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
		m_printer.stream() << args[0].name << ": " << args[0].type->toString();
	}

	for (size_t i = 1; i < args.size(); i++) {
		m_printer.stream() << ", ";
		m_printer.stream() << args[i].name << ": " << args[i].type->toString();
	}

	m_printer.stream() << ')';
	if (auto returnType = node.getReturnType(); !returnType->isNoType()) {
		m_printer.stream() << ": " << returnType->toString();
	}

	m_printer.stream() << ' ';
	if (node.isNative()) {
		m_printer.stream() << "= native(\"" << node.getBodyAsNative() << "\")";
	} else {
		Parent::visit(node.getBodyAsStatement());
	}

	m_printer.stream() << "\n\n";
}

void ast_visitor::AstPrinter::visit(ast::ModuleDeclarations& node) {
	for (auto declaration : node.getDeclarations()) {
		Parent::visit(declaration);
	}
}
