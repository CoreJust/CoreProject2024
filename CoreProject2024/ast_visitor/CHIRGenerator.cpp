// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CHIRGenerator.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "error/ErrorPrinter.hpp"
#include "symbol/SymbolAllocator.hpp"
#include "ast/AstClassImplementations.hpp"
#include "chir/ChirClassImplementations.hpp"
#include "chir/ChirAllocator.hpp"

ast_visitor::CHIRGenerator::CHIRGenerator(symbol::SymbolTable& symbols) noexcept
	: Parent(*this), m_symbols(symbols) { }

chir::Module ast_visitor::CHIRGenerator::generateCHIRModule(utils::NoNull<ast::Declaration> node) {
	Parent::visit(node);

	return chir::Module(std::move(m_declarations));
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::LiteralValue& node) {
	return chir::ChirAllocator::make<chir::ConstantValue>(node.getPosition(), node.parseAsI64());
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::IdentifierValue& node) {
	std::vector<utils::NoNull<symbol::Symbol>> symbols = m_symbols.getSymbols(node.getIdentifier());
	if (symbols.empty()) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::UNRESOLVED_SYMBOL,
			.name = "Semantic error: Unresolved symbol",
			.selectionStart = node.getPosition(),
			.selectionLength = node.getIdentifier().size(),
			.description = std::format("Failed to find symbol {}.", node.getIdentifier()),
			.explanation = "Maybe you forgot to declare this symbol or made a mistake in its name."
		});

		return nullptr;
	} else if (symbols.size() > 1) {
		if (!m_typeInquire.empty()) { // Has requirements for function argument types.
			const symbol::FunctionSymbol* function = m_symbols.getFunction(node.getIdentifier(), m_typeInquire);
			if (function == nullptr) {
				error::ErrorPrinter::error({
					.code = error::ErrorCode::UNRESOLVED_SYMBOL,
					.name = "Semantic error: Unresolved symbol",
					.selectionStart = node.getPosition(),
					.selectionLength = node.getIdentifier().size(),
					.description = std::format(
						"Failed to find function {} with argument types <{}>.", 
						node.getIdentifier(),
						utils::joinToString(m_typeInquire)
					),
					.explanation = "Maybe you forgot to declare this function or made a mistake in its name, or forgot to convert argument types."
				});

				return nullptr;
			}

			return chir::ChirAllocator::make<chir::SymbolValue>(node.getPosition(), const_cast<symbol::FunctionSymbol*>(function));
		}

		error::ErrorPrinter::error({
			.code = error::ErrorCode::TOO_MANY_SYMBOLS,
			.name = "Semantic error: Cannot choose function",
			.selectionStart = node.getPosition(),
			.selectionLength = node.getIdentifier().size(),
			.description = std::format("Found multiple functions with name {}, failed to inquire which one to choose from context.", node.getIdentifier()),
			.explanation = "Specify the required function."
		});

		return nullptr;
	}

	return chir::ChirAllocator::make<chir::SymbolValue>(node.getPosition(), symbols[0]);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::InvocationOperator& node) {
	std::vector<utils::NoNull<chir::Value>> arguments;
	arguments.reserve(node.getArguments().size());

	for (auto argument : node.getArguments()) {
		arguments.push_back(Parent::visit(argument));
	}

	std::vector<symbol::Type> argumentTypes = utils::map<std::vector<symbol::Type>>(
		arguments,
		[](auto argument) -> auto {
			return argument->getValueType();
		}
	);

	m_typeInquire = std::move(argumentTypes);
	utils::NoNull<chir::Value> callee = Parent::visit(node.getCallee());
	m_typeInquire.clear();

	if (callee->getKind() != chir::NodeKind::SYMBOL_VALUE || callee.as<chir::SymbolValue>()->getSymbolKind() != symbol::FUNCTION) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INVALID_CALLEE,
			.name = "Semantic error: Invalid callee",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = "A callee must be a function, but a non-callable expression was encountered.",
			.explanation = "-"
		});

		return nullptr;
	}

	return chir::ChirAllocator::make<chir::InvocationOperator>(
		node.getPosition(),
		callee,
		std::move(arguments),
		(callee).as<chir::SymbolValue>()->getFunction().getReturnType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::UnaryOperator& node) {
	utils::NoNull<chir::Value> value = Parent::visit(node.getExpression());
	if (value->getValueType() != symbol::TypeKind::I32) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = std::format("Expression type was expected to be i32, but {} was found.", value->getValueType().toString()),
			.explanation = "-"
		});

		return nullptr;
	}

	return chir::ChirAllocator::make<chir::UnaryOperator>(
		node.getPosition(),
		static_cast<chir::UnaryOperator::UnaryOperatorType>(node.getOperator()),
		value,
		value->getValueType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::BinaryOperator& node) {
	utils::NoNull<chir::Value> left = Parent::visit(node.getLeft());
	utils::NoNull<chir::Value> right = Parent::visit(node.getRight());
	if (left->getValueType() != right->getValueType()) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = std::format("Expression types in binary operator must match, but {} and {} were found.", left->getValueType().toString(), right->getValueType().toString()),
			.explanation = "-"
		});

		return nullptr;
	}

	if (left->getValueType() != symbol::TypeKind::I32) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = std::format("Expression type was expected to be i32, but {} was found.", left->getValueType().toString()),
			.explanation = "-"
		});

		return nullptr;
	}

	return chir::ChirAllocator::make<chir::BinaryOperator>(
		node.getPosition(),
		static_cast<chir::BinaryOperator::BinaryOperatorType>(node.getOperator()),
		left,
		right,
		left->getValueType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::ReturnOperator& node) {
	symbol::FunctionSymbol* function = m_symbols.getCurrentScope().getFunction();
	if (function == nullptr) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::NON_FUNCTION_CONTEXT,
			.name = "Semantic error: Return operator in non-function context",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = "Return operator unexpected out of function context.",
			.explanation = "-"
		});

		return nullptr;
	}

	if (node.getExpression() != nullptr) {
		utils::NoNull<chir::Value> value = Parent::visit(node.getExpression());
		if (value->getValueType() != function->getReturnType()) {
			error::ErrorPrinter::error({
				.code = error::ErrorCode::TYPE_MISMATCH,
				.name = "Semantic error: Type mismatch",
				.selectionStart = node.getPosition(),
				.selectionLength = 0,
				.description = std::format(
					"Return operator type and function type mismatch: {} and {}.", 
					value->getValueType().toString(), 
					function->getReturnType().toString()
				),
				.explanation = "-"
			});

			return nullptr;
		}

		return chir::ChirAllocator::make<chir::ReturnOperator>(node.getPosition(), value);
	}

	if (function->getReturnType() != symbol::TypeKind::UNIT) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = std::format(
				"Return operator type and function type mismatch: unit and {}.",
				function->getReturnType().toString()
			),
			.explanation = "Add a return value."
		});

		return nullptr;
	}

	return chir::ChirAllocator::make<chir::ReturnOperator>(node.getPosition());
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::ExpressionStatement& node) {
	return chir::ChirAllocator::make<chir::ValueStatement>(node.getPosition(), Parent::visit(node.getExpression())).get();
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::ScopeStatement& node) {
	std::vector<utils::NoNull<chir::Statement>> statements;
	statements.reserve(node.getStatements().size());

	// Functions create their own scope, so no new scope is needed to be created.
	bool needOwnScope = node.getParent()->getType() != ast::NodeKind::FUNCTION_DECLARATION;

	if (needOwnScope) {
		m_symbols.pushScope("");
	}

	for (auto statement : node.getStatements()) {
		chir::Statement* chirStatement = Parent::visit(statement);
		if (chirStatement != nullptr) {
			statements.push_back(chirStatement);
		}
	}

	if (needOwnScope) {
		m_symbols.popScope();
	}

	return chir::ChirAllocator::make<chir::ScopeStatement>(node.getPosition(), std::move(statements)).get();
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::VariableDeclaration& node) {
	utils::NoNull<chir::Value> initialValue = Parent::visit(node.getInitialValue());
	symbol::Type type = node.getVariableType().makeSymbolType();
	if (!type.isNoType() && initialValue->getValueType() != type) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.selectionLength = 0,
			.description = std::format(
				"Variable type and variable initial value mismatch: {} and {}.",
				initialValue->getValueType().toString(),
				type.toString()
			),
			.explanation = "-"
		});

		return nullptr;
	}

	if (m_symbols.getCurrentScope().getFunction() == nullptr) { // Global variable
		const symbol::VariableSymbol* variable = m_symbols.getVariable(node.getName());

		assert(variable != nullptr);
		assert(variable->getType() == type);

		m_declarations.emplace_back(chir::ChirAllocator::make<chir::VariableDeclaration>(
			node.getPosition(),
			*variable,
			initialValue
		));

		return nullptr; // Not a CHIR statement.
	} else { // Local variable
		if (type.isNoType()) {
			type = initialValue->getValueType(); // Local variable type inquirement.
		}

		symbol::VariableSymbol& variable = m_symbols.addVariable(symbol::SymbolPath { }, utf::String(node.getName()), type);
		return chir::ChirAllocator::make<chir::VariableStatement>(
			node.getPosition(),
			variable,
			initialValue
		).get();
	}
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::FunctionDeclaration& node) {
	symbol::FunctionSymbol* function;
	symbol::FunctionSymbol* externalFunction = m_symbols.getCurrentScope().getFunction();

	if (externalFunction == nullptr) { // Global function.
		std::vector<symbol::Type> argumentTypes = utils::map<std::vector<symbol::Type>>(node.getArguments(), [](const auto& argument) -> auto {
			return argument.type.makeSymbolType();
		});

		function = const_cast<symbol::FunctionSymbol*>(m_symbols.getFunction(node.getName(), argumentTypes));

		assert(function != nullptr);
	} else { // Local function
		symbol::SymbolPath path = externalFunction->getSymbolPath();
		path.internalPath.path.push_back(externalFunction->getName());

		auto arguments = utils::map<std::vector<utils::NoNull<symbol::VariableSymbol>>>(
			node.getArguments(), 
			[](const auto& arg) -> auto {
				return symbol::SymbolAllocator::make<symbol::VariableSymbol>(symbol::SymbolPath { }, utf::String(arg.name), arg.type.makeSymbolType());
			}
		);

		function = &m_symbols.addFunction(path, utf::String(node.getName()), node.getReturnType().makeSymbolType(), std::move(arguments));
	}

	symbol::Scope& scope = m_symbols.pushFunctionScope(*function);

	if (node.isNative()) {
		m_declarations.emplace_back(chir::ChirAllocator::make<chir::FunctionDeclaration>(node.getPosition(), *function, utf::String(node.getBodyAsNative())));
	} else { // Not a native function.
		chir::Statement* functionBody = Parent::visit(node.getBodyAsStatement().get());
		m_declarations.emplace_back(chir::ChirAllocator::make<chir::FunctionDeclaration>(node.getPosition(), *function, functionBody));
	}

	m_symbols.popScope();
	return nullptr;
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::ModuleDeclarations& node) {
	for (auto declaration : node.getDeclarations()) {
		Parent::visit(declaration);
	}

	return nullptr;
}
