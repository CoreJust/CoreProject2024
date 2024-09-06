// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Token.hpp"
#include <format>
#include "utf/StringUtils.hpp"
#include "error/InternalAssert.hpp"

utf::String lexer::tokenType2String(TokenType type) {
	internalAssert(type <= TOKEN_TYPES_COUNT);

	static const char* TOKEN_TYPE_NAMES[] = {
		"IDENTIFIER", "NUMBER", "TEXT", "RAW_TEXT",
		"FN", "NATIVE", "RETURN", 
		"IF", "ELIF", "ELSE", "LET", "AS",
		"I8", "I16", "I32", "I64", "I128", "ISIZE",
		"U8", "U16", "U32", "U64", "U128", "USIZE",
		"CINT", "CINT", "CUINT", "CULONG", 
		"BOOL", "UNIT",
		"TRUE", "FALSE",
		"EQ", "PLUS", "MINUS", "STAR",
		"SLASH", "PERCENT", "AND", "OR",
		"EQEQ", "NOTEQ", "LESSEQ", "GREATEREQ", "LESS", "GREATER",
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
