// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Tokenizer.hpp"
#include <format>
#include <map>
#include "utils/ErrorPrinter.hpp"
#include "TokenizationUtils.hpp"

std::map<utf::StringView, TokenType> KEYWORDS = {
	{ "fn", TokenType::FN },
	{ "native", TokenType::NATIVE },
	{ "return", TokenType::RETURN },
	{ "let", TokenType::LET },
	{ "i32", TokenType::I32 },
	{ "unit", TokenType::UNIT }
};

Tokenizer::Tokenizer(utf::StringView text)
	: m_ptr(text.data()), m_end(text.data() + text.size()), m_position() {
	nextChar(); // Get the first character.
	next(); // Get the first token.
}

bool Tokenizer::consume(TokenType type) {
	if (!match(type)) {
		ErrorPrinter::error({
			.code = ErrorCode::UNEXPECTED_TOKEN,
			.name = "Syntax error: Unexpected token",
			.selectionStart = m_currentToken.position,
			.selectionLength = m_currentToken.text.size(),
			.description = std::format("Encountered unexpected token {}, while [{}] was expected.", m_currentToken.toString(), tokenType2String(type)),
			.explanation = "-"
		});

		return false;
	}

	return true;
}

bool Tokenizer::match(TokenType type) {
	if (m_currentToken.type == type) {
		next();
		return true;
	}

	return false;
}

const Token& Tokenizer::next() {
	// Shifting tokens: the current becomes the previous, while the current is to be generated.
	m_previousToken = m_currentToken;
	m_currentToken = Token::NO_TOKEN();

	// Try to get the next token, skipping whitespaces (and comments), as well as unknown characters.
	while (true) {
		if (utf::isAnyWhitespace(m_char)) {
			skipWhitespaces();
			continue;
		}

		if (m_char == 0 && m_ptr >= m_end) { // Reached end of source string.
			break;
		}

		if (tokenization_utils::isIdentifierStart(m_char)) {
			tokenizeIdentifier();
		} else if (tokenization_utils::isOperator(m_char)) {
			tokenizeOperator();
		} else if (m_char == utf::encodeUtf('"')) {
			tokenizeStringLiteral();
		} else if (utf::isDigit(m_char)) {
			tokenizeNumberLiteral();
		} else {
			ErrorPrinter::error({
				.code = ErrorCode::UNEXPECTED_CHARACTER,
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

const Token& Tokenizer::current() const {
	return m_currentToken;
}

const Token& Tokenizer::previous() const {
	return m_previousToken;
}

void Tokenizer::tokenizeIdentifier() {
	TextPosition tokenPosition = m_position;
	const char* identifierStart = getCurrentCharPtr();

	while (tokenization_utils::isIdentifierMiddle(nextChar())); // Consuming the identifier.

	const char* identifierEnd = getCurrentCharPtr();
	utf::StringView identifier(identifierStart, identifierEnd);
	if (auto it = KEYWORDS.find(identifier); it != KEYWORDS.end()) {
		m_currentToken = Token {
			.text = identifier,
			.position = tokenPosition,
			.type = it->second
		};
	} else {
		m_currentToken = Token {
			.text = identifier,
			.position = tokenPosition,
			.type = TokenType::IDENTIFIER
		};
	}
}

void Tokenizer::tokenizeNumberLiteral() {

}

void Tokenizer::tokenizeStringLiteral() {

}

void Tokenizer::tokenizeOperator() {

}

void Tokenizer::skipWhitespaces() {
	while (utf::isAnyWhitespace(nextChar())) continue;
}

utf::Char Tokenizer::nextChar() {
	if (m_ptr >= m_end) return m_char = 0;

	m_char = utf::nextChar(m_ptr);
	if (utf::isNewLine(m_char)) {
		m_position.character = 0;
		m_position.line += 1;

		if (m_ptr < m_end) { // Checking for CR+LF or LF+CR
			if ((m_char == utf::encodeUtf('\r') && *m_ptr == '\n')
				|| (m_char == utf::encodeUtf('\n') && *m_ptr == '\r')) {
				m_char = utf::nextChar(m_ptr);
			}
		}
	} else {
		m_position.character += 1;
	}

	return m_char;
}

const char* Tokenizer::getCurrentCharPtr() const {
	return m_ptr - utf::getCharSize(m_char);
}
