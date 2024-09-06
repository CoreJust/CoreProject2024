// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CHIRGenerator.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "error/ErrorPrinter.hpp"
#include "error/InternalAssert.hpp"
#include "symbol/SymbolAllocator.hpp"
#include "symbol/type/FunctionType.hpp"
#include "ast/AstClassImplementations.hpp"
#include "chir/ChirClassImplementations.hpp"
#include "chir/ChirAllocator.hpp"

ast_visitor::CHIRGenerator::CHIRGenerator(std::unique_ptr<symbol::SymbolTable> symbols) noexcept
	: Parent(*this), m_symbols(std::move(symbols)) { }

std::unique_ptr<chir::Module> ast_visitor::CHIRGenerator::generateCHIRModule(utils::NoNull<ast::Declaration> node) {
	Parent::visit(node);

	return std::make_unique<chir::Module>(std::move(m_declarations), std::move(m_symbols));
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::LiteralValue& node) {
	if (node.getType()->getKind() == ast::TypeKind::BOOL) {
		return chir::ChirAllocator::make<chir::ConstantValue>(node.getPosition(), symbol::Type::make(symbol::TypeKind::BOOL), utils::IntValue(node.parseAsBool()));
	} else {
		return chir::ChirAllocator::make<chir::ConstantValue>(node.getPosition(), node.getType()->makeSymbolType(), node.parseAsInt());
	}
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::IdentifierValue& node) {
	std::vector<utils::NoNull<symbol::Symbol>> symbols = m_symbols->getSymbols(node.getIdentifier());
	if (symbols.empty()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::UNRESOLVED_SYMBOL,
			.name = "Semantic error: Unresolved symbol",
			.selectionStart = node.getPosition(),
			.selectionLength = node.getIdentifier().size(),
			.description = std::format("Failed to find symbol {}.", node.getIdentifier()),
			.explanation = "Maybe you forgot to declare this symbol or made a mistake in its name."
		});
	} else if (symbols.size() > 1) {
		if (m_hasTypeInquire) { // Has requirements for function argument types.
			const symbol::FunctionSymbol* function = m_symbols->getFunction(node.getIdentifier(), m_typeInquire);
			if (function == nullptr) {
				error::ErrorPrinter::fatalError({
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
			}

			return chir::ChirAllocator::make<chir::SymbolValue>(node.getPosition(), const_cast<symbol::FunctionSymbol*>(function));
		}

		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::TOO_MANY_SYMBOLS,
			.name = "Semantic error: Cannot choose function",
			.selectionStart = node.getPosition(),
			.selectionLength = node.getIdentifier().size(),
			.description = std::format("Found multiple functions with name {}, failed to inquire which one to choose from context.", node.getIdentifier()),
			.explanation = "Specify the required function."
		});
	}

	return chir::ChirAllocator::make<chir::SymbolValue>(node.getPosition(), symbols[0]);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::InvocationOperator& node) {
	std::vector<utils::NoNull<chir::Value>> arguments;
	arguments.reserve(node.getArguments().size());

	// Arguments generation
	for (auto argument : node.getArguments()) {
		arguments.emplace_back(Parent::visit(argument));
	}

	auto argumentTypes = utils::map<std::vector<utils::NoNull<symbol::Type>>>(
		arguments,
		[](auto argument) -> auto {
			return argument->getValueType();
		}
	);

	m_typeInquire = std::move(argumentTypes);
	m_hasTypeInquire = true;
	utils::NoNull<chir::Value> callee = Parent::visit(node.getCallee());

	// Verification of the callee type.
	if (!callee->getValueType()->isFunctionType()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::INVALID_CALLEE,
			.name = "Semantic error: Invalid callee",
			.selectionStart = node.getPosition(),
			.description = "A callee must be a function, but a non-callable expression was encountered."
		});
	} else if (const auto& calleeArgumentTypes = callee->getValueType().as<symbol::FunctionType>()->getArgumentTypes();
			   !utils::areEqual(calleeArgumentTypes, m_typeInquire, [](const auto& argumentType, const auto& inquiredType) -> bool {
				   return argumentType->equals(*inquiredType);
				})) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::INVALID_CALLEE,
			.name = "Semantic error: Invalid callee",
			.selectionStart = node.getPosition(),
			.description = std::format(
				"Callee arguments types <{}> mismatch with the given argument types <{}>.",
				utils::joinToString(calleeArgumentTypes),
				utils::joinToString(m_typeInquire)
			)
		});
	}

	m_hasTypeInquire = false;

	// Literal type inquire
	const std::vector<utils::NoNull<symbol::Type>>& calleeTypes = callee->getValueType().as<symbol::FunctionType>()->getArgumentTypes();
	uint32_t i = 0;
	for (auto argument : arguments) {
		if (argument->getValueType()->isLiteralType()) {
			internalAssert(argument->getKind() == chir::NodeKind::CONSTANT_VALUE);

			argument.as<chir::ConstantValue>()->setIntLiteralType(calleeTypes[i]);
		}

		++i;
	}

	return chir::ChirAllocator::make<chir::InvocationOperator>(
		node.getPosition(),
		callee,
		std::move(arguments),
		callee->getValueType().as<symbol::FunctionType>()->getReturnType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::UnaryOperator& node) {
	utils::NoNull<chir::Value> value = Parent::visit(node.getExpression());
	if (node.isArithmetical() && !value->getValueType()->isArithmeticType()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.description = std::format("Expression type was expected to be i32, but {} was found.", value->getValueType()->toString())
		});
	} else if (node.isLogical() && !value->getValueType()->isBoolType()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.description = std::format("Expression type was expected to be bool, but {} was found.", value->getValueType()->toString())
		});
	}

	// Literals compile-time evaluation
	if (value->getKind() == chir::NodeKind::CONSTANT_VALUE) {
		utils::IntValue& intValue = value.as<chir::ConstantValue>()->getValueAsRef();
		switch (node.getOperator()) {
			case chir::UnaryOperator::PLUS:
				return value;
			case chir::UnaryOperator::MINUS:
				intValue = -intValue; 
				return value;
			case chir::UnaryOperator::LOGIC_NOT: 
				intValue = !intValue; 
				return value;
		default: unreachable();
		}
	}

	return chir::ChirAllocator::make<chir::UnaryOperator>(
		node.getPosition(),
		static_cast<chir::UnaryOperator::UnaryOperatorType>(node.getOperator()),
		value,
		value->getValueType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::AsOperator& node) {
	utils::NoNull<chir::Value> value = Parent::visit(node.getValue());
	utils::NoNull<symbol::Type> valueType = value->getValueType();
	utils::NoNull<symbol::Type> resultType = node.getType()->makeSymbolType();

	// Check for correct convertion.
	if (valueType->isIntegerType()) {
		if (!resultType->isIntegerType() && !resultType->isFunctionType()) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::INVALID_TYPE_CONVERTION,
				.name = "Semantic error: Invalid type convertion",
				.selectionStart = node.getPosition(),
				.description = std::format("Cannot convert {} to {} using operator as.", valueType->toString(), resultType->toString()),
				.explanation = "Integer type can only be converted to another integer type or to a function pointer type."
			});
		}
	} else if (valueType->isFunctionType()) {
		if (!resultType->isIntegerType()) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::INVALID_TYPE_CONVERTION,
				.name = "Semantic error: Invalid type convertion",
				.selectionStart = node.getPosition(),
				.description = std::format("Cannot convert {} to {} using operator as.", valueType->toString(), resultType->toString()),
				.explanation = "Function pointer type can only be converted to an integer type."
			});
		}
	} else if (valueType->isBoolType()) {
		if (!resultType->isIntegerType()) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::INVALID_TYPE_CONVERTION,
				.name = "Semantic error: Invalid type convertion",
				.selectionStart = node.getPosition(),
				.description = std::format("Cannot convert {} to {} using operator as.", valueType->toString(), resultType->toString()),
				.explanation = "Bool type can only be converted to an integer type."
			});
		}
	} else {
		error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::INVALID_TYPE_CONVERTION,
				.name = "Semantic error: Invalid type convertion",
				.selectionStart = node.getPosition(),
				.description = std::format("Cannot convert {} to {} using operator as.", valueType->toString(), resultType->toString())
		});
	}

	return chir::ChirAllocator::make<chir::AsOperator>(node.getPosition(), value, resultType);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::BinaryOperator& node) {
	utils::NoNull<chir::Value> left = Parent::visit(node.getLeft());
	utils::NoNull<chir::Value> right = Parent::visit(node.getRight());
	if (!left->getValueType()->equals(*right->getValueType())) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.description = std::format(
				"Expression types in binary operator must match, but {} and {} were found.", 
				left->getValueType()->toString(), 
				right->getValueType()->toString()
			)
		});
	}

	if (node.isArithmetical() && !left->getValueType()->isArithmeticType()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.description = std::format("Expression type in arithmetic operator expected to be arithmetical, but {} was found.", left->getValueType()->toString())
		});
	} else if (node.isLogical() && !left->getValueType()->isBoolType()) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::UNEXPECTED_TYPE,
			.name = "Semantic error: Unexpected type",
			.selectionStart = node.getPosition(),
			.description = std::format("Expression type in logical operator expected to be bool, but {} was found.", left->getValueType()->toString())
		});
	}

	// Literals compile-time evaluation
	if (left->getKind() == chir::NodeKind::CONSTANT_VALUE && right->getKind() == chir::NodeKind::CONSTANT_VALUE) {
		utils::IntValue& leftIntValue = left.as<chir::ConstantValue>()->getValueAsRef();
		utils::IntValue& rightIntValue = right.as<chir::ConstantValue>()->getValueAsRef();
		ast::BinaryOperator::BinaryOperatorType operatorType = node.getOperator();
		if (rightIntValue == 0 && (operatorType == ast::BinaryOperator::DIVIDE || operatorType == ast::BinaryOperator::REMAINDER)) { // Check for division by zero.
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::DIVISION_BY_ZERO,
				.name = "Compile-time evaluation error: Division by zero",
				.selectionStart = node.getPosition(),
				.description = std::format("Tried to divide by zero while evaluating compile-time expression: {} / 0.", leftIntValue.str())
			});
		}

		switch (operatorType) {
			case ast::BinaryOperator::PLUS: leftIntValue += rightIntValue; return left;
			case ast::BinaryOperator::MINUS: leftIntValue -= rightIntValue; return left;
			case ast::BinaryOperator::MULTIPLY: leftIntValue *= rightIntValue; return left;
			case ast::BinaryOperator::DIVIDE: leftIntValue /= rightIntValue; return left;
			case ast::BinaryOperator::REMAINDER: leftIntValue %= rightIntValue; return left;
			case ast::BinaryOperator::LOGIC_AND:
				leftIntValue = (leftIntValue && rightIntValue);
				left.as<chir::ConstantValue>()->setBoolLiteralType();
				return left;
			case ast::BinaryOperator::LOGIC_OR:
				leftIntValue = (leftIntValue || rightIntValue);
				left.as<chir::ConstantValue>()->setBoolLiteralType();
				return left;
		default: unreachable();
		}
	} else if (left->getValueType()->isLiteralType()) { // Literal type inquire
		internalAssert(left->getKind() == chir::NodeKind::CONSTANT_VALUE);

		left.as<chir::ConstantValue>()->setIntLiteralType(right->getValueType());
	} else if (right->getValueType()->isLiteralType()) { // Literal type inquire
		internalAssert(right->getKind() == chir::NodeKind::CONSTANT_VALUE);

		right.as<chir::ConstantValue>()->setIntLiteralType(left->getValueType());
	}

	return chir::ChirAllocator::make<chir::BinaryOperator>(
		node.getPosition(),
		static_cast<chir::BinaryOperator::BinaryOperatorType>(node.getOperator()),
		left,
		right,
		left->getValueType()
	);
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::ComparativeBinaryOperator& node) {
	const std::vector<ast::ComparativeBinaryOperator::ComparativeOperatorType>& operators = node.getOperators();
	const std::vector<utils::NoNull<ast::Expression>>& expressions = node.getExpressions();

	utils::NoNull<chir::Value> left = Parent::visit(expressions[0]);
	chir::Value* result = nullptr;
	for (uint32_t i = 0; i < operators.size(); ++i) {
		utils::NoNull<chir::Value> right = Parent::visit(expressions[i + 1]);
		if (!left->getValueType()->equals(*right->getValueType())) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::TYPE_MISMATCH,
				.name = "Semantic error: Type mismatch",
				.selectionStart = node.getPosition(),
				.description = std::format(
					"Expression types in comparative binary operator must match, but {} and {} were found.", 
					left->getValueType()->toString(), 
					right->getValueType()->toString()
				)
			});
		}

		if (left->getValueType()->getTypeKind() == symbol::TypeKind::UNIT) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::UNEXPECTED_TYPE,
				.name = "Semantic error: Unexpected type",
				.selectionStart = node.getPosition(),
				.description = std::format("Expression type in comparative operator cannot be unit.")
			});
		}

		chir::Value* comparisonValue;

		// Literals compile-time evaluation
		if (left->getKind() == chir::NodeKind::CONSTANT_VALUE && right->getKind() == chir::NodeKind::CONSTANT_VALUE) {
			utils::IntValue& leftIntValue = left.as<chir::ConstantValue>()->getValueAsRef();
			utils::IntValue& rightIntValue = right.as<chir::ConstantValue>()->getValueAsRef();
			switch (operators[i]) {
				case ast::ComparativeBinaryOperator::EQUALS:		leftIntValue = (leftIntValue == rightIntValue); break;
				case ast::ComparativeBinaryOperator::NOT_EQUALS:	leftIntValue = (leftIntValue != rightIntValue); break;
				case ast::ComparativeBinaryOperator::LESS_EQUALS:	leftIntValue = (leftIntValue <= rightIntValue); break;
				case ast::ComparativeBinaryOperator::GREATER_EQUALS: leftIntValue = (leftIntValue >= rightIntValue); break;
				case ast::ComparativeBinaryOperator::LESS:			leftIntValue = (leftIntValue < rightIntValue); break;
				case ast::ComparativeBinaryOperator::GREATER:		leftIntValue = (leftIntValue > rightIntValue); break;
			default: unreachable();
			}

			left.as<chir::ConstantValue>()->setBoolLiteralType();
			comparisonValue = left.get();
		} else {
			// Literal type inquire
			if (left->getValueType()->isLiteralType()) {
				internalAssert(left->getKind() == chir::NodeKind::CONSTANT_VALUE);

				left.as<chir::ConstantValue>()->setIntLiteralType(right->getValueType());
			} else if (right->getValueType()->isLiteralType()) {
				internalAssert(right->getKind() == chir::NodeKind::CONSTANT_VALUE);

				right.as<chir::ConstantValue>()->setIntLiteralType(left->getValueType());
			}

			comparisonValue = chir::ChirAllocator::make<chir::BinaryOperator>(
				node.getPosition(),
				static_cast<chir::BinaryOperator::BinaryOperatorType>(operators[i] + chir::BinaryOperator::EQUALS),
				left,
				right,
				symbol::Type::make(symbol::TypeKind::BOOL)
			).get();
		}

		left = right;
		if (result == nullptr) {
			result = comparisonValue;
		} else {
			// Literals compile-time evaluation
			// Here it is enough to know only one to simplify the expression.
			if (result->getKind() == chir::NodeKind::CONSTANT_VALUE && comparisonValue->getKind() == chir::NodeKind::CONSTANT_VALUE) {
				utils::IntValue& leftIntValue = reinterpret_cast<chir::ConstantValue*>(result)->getValueAsRef();
				utils::IntValue& rightIntValue = reinterpret_cast<chir::ConstantValue*>(comparisonValue)->getValueAsRef();
				leftIntValue = (leftIntValue && rightIntValue);
				continue;
			} else if (result->getKind() == chir::NodeKind::CONSTANT_VALUE) {
				utils::IntValue& intValue = reinterpret_cast<chir::ConstantValue*>(result)->getValueAsRef();
				if (intValue) { // true && something == something
					result = comparisonValue;
				} else {
					intValue = false; // false && something == false
				}

				continue;
			} else if (comparisonValue->getKind() == chir::NodeKind::CONSTANT_VALUE) {
				utils::IntValue& intValue = reinterpret_cast<chir::ConstantValue*>(comparisonValue)->getValueAsRef();
				if (!intValue) { // something && false == false
					intValue = false; // false && something == false
					result = comparisonValue;
				}

				continue; // something && true == something
			}

			result = chir::ChirAllocator::make<chir::BinaryOperator>(
				node.getPosition(),
				chir::BinaryOperator::LOGICAL_AND,
				result,
				comparisonValue,
				symbol::Type::make(symbol::TypeKind::BOOL)
			).get();
		}
	}

	return result;
}

utils::NoNull<chir::Value> ast_visitor::CHIRGenerator::visit(ast::ReturnOperator& node) {
	if (node.getExpression() != nullptr && node.getExpression()->getKind() == ast::NodeKind::RETURN_OPERATOR) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::RETURN_IN_RETURN,
			.name = "Semantic error: Return operator within return",
			.selectionStart = node.getPosition(),
			.description = "Return operator cannot be used as the return value.",
			.explanation = "Possibly you written double return keyword instead of one."
		});
	}

	symbol::FunctionSymbol* function = m_symbols->getCurrentScope().getFunction();
	if (function == nullptr) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::NON_FUNCTION_CONTEXT,
			.name = "Semantic error: Return operator in non-function context",
			.selectionStart = node.getPosition(),
			.description = "Return operator unexpected out of function context."
		});
	}

	if (node.getExpression() != nullptr) {
		utils::NoNull<chir::Value> value = Parent::visit(node.getExpression());
		utils::NoNull<symbol::Type> functionReturnType = function->getReturnType();

		// Literal type inquire
		if (value->getValueType()->isLiteralType() && functionReturnType->isArithmeticType()) {
			internalAssert(value->getKind() == chir::NodeKind::CONSTANT_VALUE);

			value.as<chir::ConstantValue>()->setIntLiteralType(functionReturnType);
		}

		if (!value->getValueType()->equals(*functionReturnType)) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::TYPE_MISMATCH,
				.name = "Semantic error: Type mismatch",
				.selectionStart = node.getPosition(),
				.description = std::format(
					"Return operator type and function type mismatch: {} and {}.", 
					value->getValueType()->toString(), 
					function->getReturnType()->toString()
				)
			});
		}

		return chir::ChirAllocator::make<chir::ReturnOperator>(node.getPosition(), value);
	}

	if (function->getReturnType()->getTypeKind() != symbol::TypeKind::UNIT) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.description = std::format(
				"Return operator type and function type mismatch: unit and {}.",
				function->getReturnType()->toString()
			),
			.explanation = "Add a return value."
		});
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
	bool needOwnScope = node.getParent()->getKind() != ast::NodeKind::FUNCTION_DECLARATION;

	if (needOwnScope) {
		m_symbols->pushScope("");
	}

	for (auto statement : node.getStatements()) {
		chir::Statement* chirStatement = Parent::visit(statement);
		if (chirStatement != nullptr) {
			statements.push_back(chirStatement);
		}
	}

	if (needOwnScope) {
		m_symbols->popScope();
	}

	return chir::ChirAllocator::make<chir::ScopeStatement>(node.getPosition(), std::move(statements)).get();
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::IfElseStatement& node) {
	const std::vector<utils::NoNull<ast::Expression>>& conditions = node.getConditions();
	const std::vector<utils::NoNull<ast::Statement>>& ifBodies = node.getIfBodies();

	std::vector<utils::NoNull<chir::Value>> resultingConditions;
	std::vector<utils::NoNull<chir::Statement>> resultingIfBodies;

	resultingConditions.reserve(conditions.size());
	resultingIfBodies.reserve(ifBodies.size());

	for (uint32_t i = 0; i < conditions.size(); i++) {
		resultingConditions.emplace_back(Parent::visit(conditions[i]));
		resultingIfBodies.emplace_back(Parent::visit(ifBodies[i]));

		if (!resultingConditions.back()->getValueType()->isBoolType()) {
			error::ErrorPrinter::error({
				.code = error::ErrorCode::BOOL_TYPE_REQUIRED,
				.name = "Semantic error: Expected a bool type in condition context",
				.selectionStart = node.getPosition(),
				.description = std::format(
					"The condition of the if statement must be of bool type, but type {} was encountered.",
					resultingConditions.back()->getValueType()->toString()
				)
			});
		}
	}

	chir::Statement* elseBody = nullptr;
	if (node.hasElse()) {
		elseBody = Parent::visit(node.getElseBody());
	}

	return chir::ChirAllocator::make<chir::IfElseStatement>(node.getPosition(), std::move(resultingConditions), std::move(resultingIfBodies), elseBody).get();
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::VariableDeclaration& node) {
	utils::NoNull<chir::Value> initialValue = Parent::visit(node.getInitialValue());
	utils::NoNull<symbol::Type> type = node.getVariableType()->makeSymbolType();

	if (!type->isNoType() && !initialValue->getValueType()->equals(*type)) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::TYPE_MISMATCH,
			.name = "Semantic error: Type mismatch",
			.selectionStart = node.getPosition(),
			.description = std::format(
				"Variable type and variable initial value mismatch: {} and {}.",
				initialValue->getValueType()->toString(),
				type->toString()
			)
		});
	}

	// Literal type inquire
	if (initialValue->getValueType()->isLiteralType()) {
		internalAssert(initialValue->getKind() == chir::NodeKind::CONSTANT_VALUE);

		if (type->isIntegerType()) {
			initialValue.as<chir::ConstantValue>()->setIntLiteralType(type);
		} else if (type->isNoType()) {
			initialValue.as<chir::ConstantValue>()->assignOwnLiteralType();
		}
	}

	if (m_symbols->getCurrentScope().getFunction() == nullptr) { // Global variable
		const symbol::VariableSymbol* variable = m_symbols->getVariable(node.getName());

		internalAssert(variable != nullptr);
		internalAssert(variable->getType() == type);

		m_declarations.emplace_back(chir::ChirAllocator::make<chir::VariableDeclaration>(
			node.getPosition(),
			*variable,
			initialValue
		));

		return nullptr; // Not a CHIR statement.
	} else { // Local variable
		if (type->isNoType()) {
			type = initialValue->getValueType(); // Local variable type inquirement.
		}

		symbol::VariableSymbol& variable = m_symbols->addVariable(symbol::SymbolPath { }, utf::String(node.getName()), type);
		return chir::ChirAllocator::make<chir::VariableStatement>(
			node.getPosition(),
			variable,
			initialValue
		).get();
	}
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::FunctionDeclaration& node) {
	symbol::FunctionSymbol* function;
	symbol::FunctionSymbol* externalFunction = m_symbols->getCurrentScope().getFunction();

	if (externalFunction == nullptr) { // Global function.
		auto argumentTypes = utils::map<std::vector<utils::NoNull<symbol::Type>>>(node.getArguments(), [](const auto& argument) -> auto {
			return argument.type->makeSymbolType();
		});

		function = const_cast<symbol::FunctionSymbol*>(m_symbols->getFunction(node.getName(), argumentTypes));

		internalAssert(function != nullptr);
	} else { // Local function
		symbol::SymbolPath path = externalFunction->getSymbolPath();
		path.internalPath.path.push_back(externalFunction->getName());

		auto arguments = utils::map<std::vector<utils::NoNull<symbol::VariableSymbol>>>(
			node.getArguments(), 
			[](const auto& arg) -> auto {
				return symbol::SymbolAllocator::make<symbol::VariableSymbol>(symbol::SymbolPath { }, utf::String(arg.name), arg.type->makeSymbolType());
			}
		);

		function = &m_symbols->addFunction(path, utf::String(node.getName()), node.getReturnType()->makeSymbolType(), std::move(arguments));
	}

	symbol::Scope& scope = m_symbols->pushFunctionScope(*function);

	if (node.isNative()) {
		m_declarations.emplace_back(chir::ChirAllocator::make<chir::FunctionDeclaration>(node.getPosition(), *function, utf::String(node.getBodyAsNative())));
	} else { // Not a native function.
		chir::Statement* functionBody = Parent::visit(node.getBodyAsStatement().get());
		m_declarations.emplace_back(chir::ChirAllocator::make<chir::FunctionDeclaration>(node.getPosition(), *function, functionBody));
	}

	m_symbols->popScope();
	return nullptr;
}

chir::Statement* ast_visitor::CHIRGenerator::visit(ast::ModuleDeclarations& node) {
	for (auto declaration : node.getDeclarations()) {
		Parent::visit(declaration);
	}

	return nullptr;
}
