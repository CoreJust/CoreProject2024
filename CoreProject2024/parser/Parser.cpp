// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Parser.hpp"
#include <format>
#include "error/ErrorPrinter.hpp"
#include "ast/AstAllocator.hpp"
#include "ast/AstClassImplementations.hpp"

parser::Parser::Parser(lexer::Tokenizer tokenizer) : m_toks(std::move(tokenizer)) { }

ast::Declaration* parser::Parser::parse() {
	std::vector<utils::NoNull<ast::Declaration>> result;
	m_toks.match(lexer::NEWLINE); // Skipping possible initial new line.

	while (!m_toks.current().isNoToken()) {
		if (m_toks.match(lexer::FN)) {
			result.push_back(functionDeclaration());
		} else if (m_toks.match(lexer::LET)) {
			result.push_back(variableDeclaration());
		} else {
			error::ErrorPrinter::error({
				.code = error::ErrorCode::EXPECTED_A_DECLARATION,
				.name = "Syntax error: Expected a declaration",
				.selectionStart = m_toks.current().position,
				.selectionLength = m_toks.current().text.size(),
				.description = std::format("Encountered unexpected token {}, while a global declaration was expected.",  m_toks.current().toString()),
			});

			m_toks.next();
			continue;
		}

		m_toks.consumeRange(lexer::NEWLINE, lexer::NO_TOKEN_TYPE, "Declarations must begin with a new line.");
	}

	return ast::AstAllocator::make<ast::ModuleDeclarations>(utils::TextPosition(), std::move(result)).get();
}

ast::Declaration* parser::Parser::functionDeclaration() {
	const utils::TextPosition position = m_toks.current().position;
	const utf::StringView name = m_toks.consume(lexer::IDENTIFIER, "Missing function name, add a name to the function declaration.").text;
	std::vector<ast::FunctionDeclaration::Argument> arguments;

	m_toks.consume(lexer::LPAREN, "Function must have an argument list that begins with a '('.");
	while (!m_toks.match(lexer::RPAREN)) {
		const utf::StringView argName = m_toks.consume(lexer::IDENTIFIER, "Expected argument name, function argument must have format <name>: <type>.").text;
		m_toks.consume(lexer::COLON, "Expected ':' for function argument type, function argument must have format <name>: <type>.");

		ast::Type argType = parseType();
		arguments.emplace_back(ast::FunctionDeclaration::Argument{ .name = argName, .type = std::move(argType) });
		if (!m_toks.match(lexer::COMMA)) {
			m_toks.consume(lexer::RPAREN, "Function argument list must end with ')', close the argument list.");
			break;
		}
	}

	ast::Type returnType = ast::Type("unit");
	if (m_toks.match(lexer::COLON)) {
		returnType = parseType();
	}

	// Function body
	if (m_toks.match(lexer::EQ)) {
		if (m_toks.match(lexer::NATIVE)) { // Native function declaration
			m_toks.consume(lexer::LPAREN, "Native function names must be written in parens, format fn ... = native(\"<native name>\").");
			const utf::StringView nativeName = m_toks.consumeRange(lexer::TEXT, lexer::RAW_TEXT, "Missing native function name, format fn ... = native(\"<native name>\").").text;
			m_toks.consume(lexer::RPAREN, "Missing closing ')' after native function name, expected format fn ... = native(\"<native name>\").");

			return ast::AstAllocator::make<ast::FunctionDeclaration>(position, name, std::move(returnType), std::move(arguments), nativeName).get();
		} else { // = <return expression>
			utils::NoNull<ast::ReturnOperator> returnOperator = ast::AstAllocator::make<ast::ReturnOperator>(m_toks.current().position, expression());
			utils::NoNull<ast::ExpressionStatement> returnStatement = ast::AstAllocator::make<ast::ExpressionStatement>(m_toks.current().position, returnOperator);
			utils::NoNull<ast::ScopeStatement> functionBody = ast::AstAllocator::make<ast::ScopeStatement>(
				m_toks.current().position, 
				std::vector<utils::NoNull<ast::Statement>>{ returnStatement }
			);

			return ast::AstAllocator::make<ast::FunctionDeclaration>(position, name, std::move(returnType), std::move(arguments), functionBody).get();
		}
	}

	// Function with scope body
	m_toks.consume(lexer::LBRACE, "Function body must begin with either '=' or '{'.");
	return ast::AstAllocator::make<ast::FunctionDeclaration>(position, name, std::move(returnType), std::move(arguments), scopeStatement()).get();
}

ast::Declaration* parser::Parser::variableDeclaration() {
	const utils::TextPosition position = m_toks.current().position;
	const utf::StringView name = m_toks.consume(lexer::IDENTIFIER, "Missing variable name, add a name to the variable.").text;
	ast::Type type;
	if (m_toks.match(lexer::COLON)) {
		type = parseType();
	}

	m_toks.consume(lexer::EQ, "Missing '=', variable must be initialized with initial value.");
	ast::Expression* initialValue = expression();

	return ast::AstAllocator::make<ast::VariableDeclaration>(position, name, std::move(type), initialValue).get();
}

ast::Statement* parser::Parser::statement() {
	if (m_toks.match(lexer::FN)) {
		return functionDeclaration();
	} else if (m_toks.match(lexer::LET)) {
		return variableDeclaration();
	} else if (m_toks.match(lexer::RBRACE)) {
		return scopeStatement();
	} else {
		ast::Expression* expr = expression();
		if (expr != nullptr) {
			return ast::AstAllocator::make<ast::ExpressionStatement>(expr->getPosition(), expr).get();
		}

		error::ErrorPrinter::error({
			.code = error::ErrorCode::EXPECTED_A_STATEMENT,
			.name = "Syntax error: Expected a statement",
			.selectionStart = m_toks.current().position,
			.selectionLength = m_toks.current().text.size(),
			.description = std::format("Encountered unexpected token {}, while a statement was expected.", m_toks.current().toString()),
		});

		return nullptr;
	}
}

ast::Statement* parser::Parser::scopeStatement() {
	const utils::TextPosition position = m_toks.current().position;
	std::vector<utils::NoNull<ast::Statement>> statements;
	m_toks.match(lexer::NEWLINE);

	while (!m_toks.match(lexer::RBRACE)) {
		statements.push_back(statement());

		if (!m_toks.current().isNoToken()) {
			m_toks.consumeRange(lexer::SEMICOLON, lexer::NEWLINE, "Statements must be separated with a new line (or with ';').");
		} else {
			error::ErrorPrinter::error({
				.code = error::ErrorCode::NO_CLOSING_BRACE,
				.name = "Syntax error: No closing brace '}'",
				.selectionStart = m_toks.current().position,
				.selectionLength = m_toks.current().text.size(),
				.description = "Encountered unexpected end of file, when looking for the closing right brace.",
			});

			break;
		}
	}

	return ast::AstAllocator::make<ast::ScopeStatement>(position, std::move(statements)).get();
}

ast::Expression* parser::Parser::expression() {
	if (m_toks.match(lexer::RETURN)) {
		return returnOperator();
	}

	return logical();
}

ast::Expression* parser::Parser::returnOperator() {
	const utils::TextPosition position = m_toks.current().position;
	if (m_toks.matchRange(lexer::SEMICOLON, lexer::NO_TOKEN_TYPE)) {
		return ast::AstAllocator::make<ast::ReturnOperator>(position).get();
	} else {
		return ast::AstAllocator::make<ast::ReturnOperator>(position, expression()).get();
	}
}

ast::Expression* parser::Parser::logical() {
	ast::Expression* result = comparative();
	while (true) {
		if (m_toks.match(lexer::LOGIC_AND)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::LOGIC_AND, result, comparative()).get();
			continue;
		} else if (m_toks.match(lexer::LOGIC_OR)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::LOGIC_OR, result, comparative()).get();
			continue;
		}

		break;
	}

	return result;
}

ast::Expression* parser::Parser::comparative() {
	static thread_local std::vector<utils::NoNull<ast::Expression>> s_comparedExpressions;
	static thread_local std::vector<ast::ComparativeBinaryOperator::ComparativeOperatorType> s_operators;

	ast::Expression* result = additive();
	s_comparedExpressions.clear();
	s_comparedExpressions.emplace_back(result);

	while (true) {
		if (m_toks.match(lexer::EQEQ)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::EQUALS);
			continue;
		} else if (m_toks.match(lexer::NOTEQ)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::NOT_EQUALS);
			continue;
		} else if (m_toks.match(lexer::LESSEQ)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::LESS_EQUALS);
			continue;
		} else if (m_toks.match(lexer::GREATEREQ)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::GREATER_EQUALS);
			continue;
		} else if (m_toks.match(lexer::LESS)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::LESS);
			continue;
		} else if (m_toks.match(lexer::GREATER)) {
			s_comparedExpressions.emplace_back(additive());
			s_operators.emplace_back(ast::ComparativeBinaryOperator::GREATER);
			continue;
		}

		break;
	}

	// Now check if there were no comparisons.
	if (s_operators.empty()) {
		return result;
	}

	// There was at least one comparison.
	return ast::AstAllocator::make<ast::ComparativeBinaryOperator>(result->getPosition(), std::exchange(s_operators, { }), std::exchange(s_comparedExpressions, { })).get();
}

ast::Expression* parser::Parser::additive() {
	ast::Expression* result = multiplicative();
	while (true) {
		if (m_toks.match(lexer::PLUS)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::PLUS, result, multiplicative()).get();
			continue;
		} else if (m_toks.match(lexer::MINUS)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::MINUS, result, multiplicative()).get();
			continue;
		}

		break;
	}

	return result;
}

ast::Expression* parser::Parser::multiplicative() {
	ast::Expression* result = unary();
	while (true) {
		if (m_toks.match(lexer::STAR)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::MULTIPLY, result, unary()).get();
			continue;
		} else if (m_toks.match(lexer::SLASH)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::DIVIDE, result, unary()).get();
			continue;
		} else if (m_toks.match(lexer::PERCENT)) {
			result = ast::AstAllocator::make<ast::BinaryOperator>(m_toks.current().position, ast::BinaryOperator::REMAINDER, result, unary()).get();
			continue;
		}

		break;
	}

	return result;
}

ast::Expression* parser::Parser::unary() {
	if (m_toks.match(lexer::PLUS)) {
		return ast::AstAllocator::make<ast::UnaryOperator>(m_toks.current().position, ast::UnaryOperator::PLUS, unary()).get();
	} else if (m_toks.match(lexer::MINUS)) {
		return ast::AstAllocator::make<ast::UnaryOperator>(m_toks.current().position, ast::UnaryOperator::MINUS, unary()).get();
	} else if (m_toks.match(lexer::NOT)) {
		return ast::AstAllocator::make<ast::UnaryOperator>(m_toks.current().position, ast::UnaryOperator::LOGIC_NOT, unary()).get();
	}

	return postprimary();
}

ast::Expression* parser::Parser::postprimary() {
	ast::Expression* result = primary();
	while (true) {
		if (m_toks.match(lexer::LPAREN)) { // Invocation
			std::vector<utils::NoNull<ast::Expression>> arguments;
			while (!m_toks.match(lexer::RPAREN)) {
				arguments.push_back(expression());

				if (!m_toks.match(lexer::COMMA)) {
					m_toks.consume(lexer::RPAREN, "Missing closing ')' after the argument list of function invocation.");
					break;
				}
			}

			result = ast::AstAllocator::make<ast::InvocationOperator>(m_toks.current().position, result, std::move(arguments)).get();
			continue;
		}

		break;
	}

	return result;
}

ast::Expression* parser::Parser::primary() {
	if (m_toks.match(lexer::LPAREN)) {
		ast::Expression* result = expression();
		m_toks.consume(lexer::RPAREN, "Missing closing ')' after expression in parens.");

		return result;
	}

	utf::StringView tokenText = m_toks.current().text;
	utils::TextPosition position = m_toks.current().position;
	if (m_toks.match(lexer::NUMBER)) {
		return ast::AstAllocator::make<ast::LiteralValue>(position, tokenText).get();
	} else if (m_toks.match(lexer::IDENTIFIER)) {
		return ast::AstAllocator::make<ast::IdentifierValue>(position, tokenText).get();
	} else if (m_toks.match(lexer::FALSE)) {
		return ast::AstAllocator::make<ast::LiteralValue>(position, false).get();
	} else if (m_toks.match(lexer::TRUE)) {
		return ast::AstAllocator::make<ast::LiteralValue>(position, true).get();
	}

	return nullptr;
}

ast::Type parser::Parser::parseType() {
	if (m_toks.match(lexer::I32)) {
		return ast::Type("i32");
	} else if (m_toks.match(lexer::BOOL)) {
		return ast::Type("bool");
	} else if (m_toks.match(lexer::UNIT)) {
		return ast::Type("unit");
	} else {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::EXPECTED_A_TYPE,
			.name = "Syntax error: Expected a type",
			.selectionStart = m_toks.current().position,
			.selectionLength = m_toks.current().text.size(),
			.description = std::format("Encountered unexpected token {}, while a type was expected.",  m_toks.current().toString()),
		});

		return ast::Type();
	}
}
