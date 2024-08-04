// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Tokenizer.hpp"
#include <cassert>
#include <format>
#include "utf/StringHash.hpp"
#include "error/ErrorPrinter.hpp"
#include "TokenizationUtils.hpp"

using lexer::TokenType;
using lexer::Token;

// Returns the keyword TokenType corresponding to the string hashed with hashBuilder.
// If no keyword matches is, returns NO_TOKEN_TYPE.
constexpr TokenType getKeywordTokenType(const utils::StringHashBuilder& hashBuilder) noexcept {
	switch (hashBuilder.getHash()) {
		case utils::hashOf("fn"): return TokenType::FN;
		case utils::hashOf("native"): return TokenType::NATIVE;
		case utils::hashOf("return"): return TokenType::RETURN;
		case utils::hashOf("let"): return TokenType::LET;
		case utils::hashOf("i32"): return TokenType::I32;
		case utils::hashOf("unit"): return TokenType::UNIT;
	default: return TokenType::NO_TOKEN_TYPE;
	}
}

// Returns the operator TokenType corresponding to the string hashed with hashBuilder.
// If no operator matches is, returns NO_TOKEN_TYPE.
constexpr TokenType getOperatorTokenType(const utils::StringHashBuilder& hashBuilder) noexcept {
	switch (hashBuilder.getHash()) {
		case utils::hashOf("="): return TokenType::EQ;
		case utils::hashOf("+"): return TokenType::PLUS;
		case utils::hashOf("-"): return TokenType::MINUS;
		case utils::hashOf("*"): return TokenType::STAR;
		case utils::hashOf("/"): return TokenType::SLASH;
		case utils::hashOf("%"): return TokenType::PERCENT;
		case utils::hashOf("("): return TokenType::LPAREN;
		case utils::hashOf(")"): return TokenType::RPAREN;
		case utils::hashOf("{"): return TokenType::LBRACE;
		case utils::hashOf("}"): return TokenType::RBRACE;
		case utils::hashOf(":"): return TokenType::COLON;
		case utils::hashOf(","): return TokenType::COMMA;
		case utils::hashOf(";"): return TokenType::SEMICOLON;
	default: return TokenType::NO_TOKEN_TYPE;
	}
}

lexer::Tokenizer::Tokenizer(utf::StringView text)
	: m_ptr(text.data()), m_end(text.data() + text.size()), m_position() {
	nextChar(); // Get the first character.
	next(); // Get the first token.
}

Token lexer::Tokenizer::consume(TokenType type, utf::String explanation) {
	Token result = m_currentToken;
	if (!match(type)) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::UNEXPECTED_TOKEN,
			.name = "Syntax error: Unexpected token",
			.selectionStart = m_currentToken.position,
			.selectionLength = m_currentToken.text.size(),
			.description = std::format("Encountered unexpected token {}, while [{}] was expected.", m_currentToken.toString(), tokenType2String(type)),
			.explanation = std::move(explanation)
		});

		return NO_TOKEN;
	}

	return result;
}

Token lexer::Tokenizer::consumeRange(TokenType first, TokenType last, utf::String explanation) {
	Token result = m_currentToken;
	if (!matchRange(first, last)) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::UNEXPECTED_TOKEN,
			.name = "Syntax error: Unexpected token",
			.selectionStart = m_currentToken.position,
			.selectionLength = m_currentToken.text.size(),
			.description = std::format("Encountered unexpected token {}, while token in range [{}..{}] was expected.", m_currentToken.toString(), tokenType2String(first), tokenType2String(last)),
			.explanation = std::move(explanation)
		});

		return NO_TOKEN;
	}

	return result;
}

bool lexer::Tokenizer::match(TokenType type) {
	if (m_currentToken.type == type) {
		next();
		return true;
	}

	return false;
}

bool lexer::Tokenizer::matchRange(TokenType first, TokenType last) {
	assert(first <= last);

	if (m_currentToken.type >= first && m_currentToken.type <= last) {
		next();
		return true;
	}

	return false;
}

const Token& lexer::Tokenizer::next() {
	// Shifting tokens: the current becomes the previous, while the current is to be generated.
	m_previousToken = m_currentToken;
	m_currentToken = NO_TOKEN;

	utils::TextPosition initialPosition = m_position;
	const char* initialPtr = getCurrentCharPtr();

	// Try to get the next token, skipping whitespaces (and comments), as well as unknown characters.
	while (true) {
		if (utf::isAnyWhitespace(m_char)) {
			skipWhitespaces();
			continue;
		}

		if (m_char == 0 && m_ptr >= m_end) { // Reached end of source string.
			break;
		}

		if (m_previousToken.type != TokenType::SEMICOLON) { // No need to produce new line token after a semicolon
			if (m_position.line != initialPosition.line || utf::isNewLine(utf::extractChar(initialPtr))) { // There was a new line(s)
				m_currentToken = Token {
					.text = utf::StringView(initialPtr, getCurrentCharPtr()),
					.position = initialPosition,
					.type = TokenType::NEWLINE
				};

				break;
			}
		}

		if (m_char == utf::encodeUtf('"') || (m_char == utf::encodeUtf('r') && peek(0) == '"')) { // Normal or raw string literal.
			tokenizeStringLiteral();
		} else if (lexer::isIdentifierStart(m_char)) {
			tokenizeIdentifier();
		} else if (lexer::isOperator(m_char)) {
			tokenizeOperator();
		} else if (utf::isDigit(m_char)) {
			tokenizeNumberLiteral();
		} else {
			error::ErrorPrinter::error({
				.code = error::ErrorCode::UNEXPECTED_CHARACTER,
				.name = "Syntax error: Unexpected character",
				.selectionStart = m_position,
				.selectionLength = 1,
				.description = std::format("Encountered unexpected character while tokenizing the source code: '{}' [code U+{:0>4X}]", utf::asString(m_char), utf::decodeUtf(m_char)),
				.explanation = "-"
			});

			nextChar();
			continue;
		}

		break;
	}

	return m_currentToken;
}

const Token& lexer::Tokenizer::current() const {
	return m_currentToken;
}

const Token& lexer::Tokenizer::previous() const {
	return m_previousToken;
}

void lexer::Tokenizer::tokenizeIdentifier() {
	utils::TextPosition tokenPosition = m_position;
	const char* identifierStart = getCurrentCharPtr();

	utils::StringHashBuilder identifierHash(m_char);
	while (lexer::isIdentifierMiddle(nextChar())) identifierHash.consume(m_char); // Consuming the identifier.

	const char* identifierEnd = getCurrentCharPtr();
	utf::StringView identifier(identifierStart, identifierEnd);
	if (auto keyword = getKeywordTokenType(identifierHash); keyword != TokenType::NO_TOKEN_TYPE) {
		m_currentToken = Token {
			.text = identifier,
			.position = tokenPosition,
			.type = keyword
		};
	} else {
		m_currentToken = Token {
			.text = identifier,
			.position = tokenPosition,
			.type = TokenType::IDENTIFIER
		};
	}
}

void lexer::Tokenizer::tokenizeNumberLiteral() {
	utils::TextPosition tokenPosition = m_position;
	const char* numberStart = getCurrentCharPtr();

	const bool canBeNumericSystemStart = (m_char == utf::encodeUtf('0'));
	nextChar(); // Skipping the first character (sure to be digit since the function was invoked).
	const bool hasNumericSystem = canBeNumericSystemStart && (m_char == utf::encodeUtf('b') || m_char == utf::encodeUtf('o') || m_char == utf::encodeUtf('x'));
	if (hasNumericSystem && !utf::isDigit(nextChar())) {
		error::ErrorPrinter::error({
				.code = error::ErrorCode::NO_NUMBER_AFTER_NUMERIC_SYSTEM,
				.name = "Syntax error: Expected a number",
				.selectionStart = tokenPosition,
				.selectionLength = 2,
				.description = std::format("Encountered numeric system, after which a number is expected, but no number was found."),
				.explanation = "-"
		});
	}

	if (utf::isDigit(m_char)) {
		while (utf::isDigit(nextChar())); // Consuming the number literal.
	}

	const char* numberEnd = getCurrentCharPtr();
	utf::StringView numberLiteral(numberStart, numberEnd);
	m_currentToken = Token {
		.text = numberLiteral,
		.position = tokenPosition,
		.type = TokenType::NUMBER
	};
}

void lexer::Tokenizer::tokenizeStringLiteral() {
	utils::TextPosition tokenPosition = m_position;
	bool isRawString = false;
	if (m_char == utf::encodeUtf('r')) { // Check for raw string.
		isRawString = true;
		nextChar();
	}

	bool isMultilineString = false;
	if (peek(0) == '"' && peek(1) == '"') { // Check for multiline string.
		nextChar();
		nextChar();
		isMultilineString = true;
	}

	const char* stringStart = m_ptr; // Not the current char ptr, but this one, because we don't need the " in the string literal.
	const char* stringEnd = m_ptr;
	int numberOfSuccessiveQoutes = 0; // Required for multiline string handling.

	while (true) { // Go on until we reach the end of string.
		// TODO: add string interpolation handling.

		if (nextChar() == utf::encodeUtf('"')) { // Check for the closing quotes.
			if (numberOfSuccessiveQoutes == isMultilineString * 2) {
				stringEnd = m_ptr - 1 - 2 * isMultilineString; // It is safe to do direct substraction because we know that " character has the size of one byte.
				nextChar(); // Skip the last quotes.
				break;
			}

			numberOfSuccessiveQoutes += 1;
		} else {
			numberOfSuccessiveQoutes = 0;
		}

		if (!isMultilineString && utf::isNewLine(m_char)) { // Reached the end of line but have not found the closing " (only for single-line strings).
			error::ErrorPrinter::error({
				.code = error::ErrorCode::NO_CLOSING_QUOTES,
				.name = "Syntax error: No closing double quotes",
				.selectionStart = tokenPosition,
				.selectionLength = 1,
				.description = std::format("Encountered unexpected end of line, but have not reached the closing double quotes for the string literal."),
				.explanation = "-"
			});

			break;
		} else if (m_ptr >= m_end) { // Reached the end of source file but have not found the closing "
			error::ErrorPrinter::error({
				.code = error::ErrorCode::NO_CLOSING_QUOTES,
				.name = "Syntax error: No closing double quotes",
				.selectionStart = tokenPosition,
				.selectionLength = 1,
				.description = std::format("Encountered unexpected end of file, but have not reached the closing double quotes for the string literal."),
				.explanation = "-"
			});

			break;
		} else if (!isRawString && m_char == utf::encodeUtf('\\') && peek(-2) != '\\' && peek(0) == '"') {
			nextChar(); // Skipping \" since it is not the end of string.
		}
	}

	if (stringEnd == stringStart) {
		stringEnd = getCurrentCharPtr(); // In case there were no closing quotes.
	}

	utf::StringView stringLiteral(stringStart, stringEnd);
	m_currentToken = Token {
		.text = stringLiteral,
		.position = tokenPosition,
		.type = static_cast<TokenType>(static_cast<uint8_t>(TokenType::TEXT) + isMultilineString)
	};
}

void lexer::Tokenizer::tokenizeOperator() {
	utils::TextPosition tokenPosition = m_position;
	const char* operatorStart = getCurrentCharPtr();

	utils::StringHashBuilder operatorHash(m_char);
	while (getOperatorTokenType(operatorHash) != TokenType::NO_TOKEN_TYPE && lexer::isOperator(nextChar()))
		operatorHash.consume(m_char); // Consuming the operator.

	TokenType operatorTokenType = getOperatorTokenType(operatorHash);
	if (operatorTokenType == TokenType::NO_TOKEN_TYPE) {
		operatorHash.unconsume(m_char);
		operatorTokenType = getOperatorTokenType(operatorHash);
	}

	utf::StringView operatorString(operatorStart, getCurrentCharPtr());
	m_currentToken = Token {
		.text = operatorString,
		.position = tokenPosition,
		.type = operatorTokenType
	};
}

void lexer::Tokenizer::skipWhitespaces() {
	while (utf::isAnyWhitespace(nextChar())) continue;
}

utf::Char lexer::Tokenizer::nextChar() {
	if (m_ptr >= m_end) {
		if (m_ptr == m_end) {
			m_ptr += 1;
		}

		return m_char = 0;
	}

	m_char = utf::nextChar(m_ptr);
	if (utf::isNewLine(m_char)) {
		// The following code is required to monitor CR+LF and LF+CR sequences so that they are considered a single new line.
		// It does not update the current line number if the current character is the second one in the sequence of one of those above.
		bool isCROrLF = (m_char == utf::encodeUtf('\n') || m_char == utf::encodeUtf('\r'));
		if (!isCROrLF || m_hadCROrLF == 0 || m_hadCROrLF == m_char) {
			m_hadCROrLF = isCROrLF * m_char;
			m_position.line += 1;
		} else {
			m_hadCROrLF = 0;
		}

		m_position.character = 0;
	} else {
		m_hadCROrLF = 0;
		m_position.character += 1;
	}

	return m_char;
}

char lexer::Tokenizer::peek(int offset) {
	if (m_ptr + offset >= m_end) {
		return '\0';
	}

	return m_ptr[offset];
}

const char* lexer::Tokenizer::getCurrentCharPtr() const {
	if (m_ptr > m_end) return m_end;

	return m_ptr - utf::getCharSize(m_char);
}
