// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Token.hpp by CoreJust
*	Created on 20.07.2024
*	Contains types for core programming language's tokens: TokenType and Token.
*/

#pragma once
#include "utils/TextPosition.hpp"

namespace lexer {
	// TokenType represents a category of a single token.
	// Some tokens represent a fixed substring in text (like keywords and operators),
	// while others represent a wide range of those (like identifiers and literals).
	enum TokenType : uint8_t {
		IDENTIFIER = 0,
		NUMBER, // number literal
		TEXT, // string literal
		RAW_TEXT, // raw string literal

		// Keywords
		FN,
		NATIVE,
		RETURN,
		LET,
		I32,
		BOOL,
		UNIT,
		TRUE,
		FALSE,

		// Operators and other symbols
		EQ, // =
		PLUS, // +
		MINUS, // -
		STAR, // *
		SLASH, // /
		PERCENT, // %
		AND, // &
		OR, // |
		EQEQ, // ==
		NOTEQ, // !=
		LESSEQ, // <=
		GREATEREQ, // >=
		LESS, // <
		GREATER, // <
		NOT, // !
		LOGIC_AND, // &&
		LOGIC_OR, // ||
		LPAREN, // (
		RPAREN, // )
		LBRACE, // {
		RBRACE, // }
		COLON, // :
		COMMA, // ,
		SEMICOLON, // ;

		NEWLINE, // Any number of new lines (probably with spaces/comments amidst)

		NO_TOKEN_TYPE,

		TOKEN_TYPES_COUNT
	};

	// Returns the token type name in the upper case.
	utf::String tokenType2String(TokenType type);

	// Represents a single token in the code.
	struct Token {
		utf::StringView text;
		utils::TextPosition position;
		TokenType type;

		// Returns true if the token is a NO_TOKEN.
		constexpr bool isNoToken() const noexcept {
			return type == NO_TOKEN_TYPE;
		}

		// Converts a token to a string representation for output.
		utf::String toString() const;
	};

	constexpr Token NO_TOKEN = Token {
		.text = utf::StringView(),
		.position = utils::TextPosition(),
		.type = NO_TOKEN_TYPE
	};
}