// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Token.hpp"
#include <cassert>
#include <format>
#include "utf/StringUtils.hpp"

utf::String lexer::tokenType2String(TokenType type) {
	assert(type <= TOKEN_TYPES_COUNT);

	static const char* TOKEN_TYPE_NAMES[] = {
		"IDENTIFIER", "NUMBER", "TEXT", "RAW_TEXT",
		"FN", "NATIVE", "RETURN", "LET",
		"I32", "BOOL", "UNIT",
		"TRUE", "FALSE",
		"EQ", "PLUS", "MINUS", "STAR",
		"SLASH", "PERCENT", "AND", "OR",
		"EQEQ", "NOTEQ", "LESSEQ", "GREATEREQ", 
		"LESS", "GREATER",
		"NOT", "LOGIC_AND", "LOGIC_OR",
		"LPAREN", "RPAREN", "LBRACE", "RBRACE",
		"COLON", "SEMICOLON", "COMMA",
		"NEWLINE", "NO_TOKEN", "TOKEN_TYPES_COUNT"
	};

	return TOKEN_TYPE_NAMES[type];
}

utf::String lexer::Token::toString() const {
	if (this->text.empty()) {
		return std::format("[{} at {}]", tokenType2String(type), position.toString());
	} else {
		return std::format("[{} \"{}\" at {}]", tokenType2String(type), utf::replaceControlCharacters(text), position.toString());
	}
}
