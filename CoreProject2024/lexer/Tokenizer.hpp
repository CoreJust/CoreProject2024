// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Tokenizer.hpp by CoreJust
*	Created on 24.07.2024
*	Contains the Tokenizer class that allows conversion of the core source file into tokens.
*/

#pragma once
#include "utils/Concept.hpp"
#include "Token.hpp"

namespace lexer {
	class Tokenizer final {
	private:
		const char* m_ptr;
		const char* m_end;
		utf::Char m_char;
		utils::TextPosition m_position;

		utf::Char m_hadCROrLF = 0; // Set to corresponding character if the previous character was CR or LF

		Token m_currentToken = NO_TOKEN;
		Token m_previousToken = NO_TOKEN;

	public:
		Tokenizer(utf::StringView text);

		// If the current token matches the given type, then just goes to the next one.
		// Otherwise, it is a syntax error, and Unexpected token error is printed.
		// Returns the consumed token (or NO_TOKEN() on failure).
		Token consume(TokenType type, utf::String explanation = "-");

		// If the current token is within the range first..last inclusively, then just goes to the next one.
		// Otherwise, it is a syntax error, and Unexpected token error is printed.
		// Returns the consumed token (or NO_TOKEN() on failure).
		Token consumeRange(TokenType first, TokenType last, utf::String explanation = "-");

		// If the current token matches the given type, then returns true and goes to the next token.
		// Otherwise, returns false.
		bool match(TokenType type);

		// If the current token is within the range first..last inclusively, then returns true and goes to the next token.
		// Otherwise, returns false.
		bool matchRange(TokenType first, TokenType last);

		// Similar to the previous match method, but uses the given predicate function.
		bool match(utils::Callable<bool, const Token&> auto pred) {
			if (pred(m_currentToken)) {
				next();
				return true;
			}

			return false;
		}

		// Tries to get the next token.
		// Updates the current and the previous tokens (the current becomes the previous, while the next token is stored into the current).
		// Returns NO_TOKEN if the end of source text is reached.
		const Token& next();

		// Returns the current token.
		// Returns NO_TOKEN if the end of source was reached.
		const Token& current() const;

		// Returns the previous token.
		// Returns NO_TOKEN if the the current token is the first one, so there is no previous token.
		const Token& previous() const;

	private:
		// Tokenizes an identifier starting from the current character and stores it to the current character.
		// It can be either an actual identifier or a keyword.
		void tokenizeIdentifier();

		// Tokenizes a number literal starting from the current character and stores it to the current character.
		void tokenizeNumberLiteral();

		// Tokenizes a string literal starting from the current character and stores it to the current character.
		void tokenizeStringLiteral();

		// Tokenizes an operator starting from the current character and stores it to the current character.
		void tokenizeOperator();

		void skipWhitespaces();

		// Returns the next character. Updates current position in the text.
		utf::Char nextChar();

		// Tries to peek forward. Works only for ASCII sequences.
		// Returns 0 if the offset goes out of the source string.
		// Use offset = 0 to look for the next character.
		char peek(int offset);

		// Returns the pointer to the current character in the text.
		const char* getCurrentCharPtr() const;
	};
}