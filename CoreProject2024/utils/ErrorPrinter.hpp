// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ErrorPrinter.hpp by CoreJust
*	Created on 25.07.2024
*	Contains the ErrorPrinter class that allows convenient error output based on the current
*	settings.
*/

#pragma once
#include "String.hpp"
#include "TextPosition.hpp"

// ErrorDetailLevel specifies the amount of information printed for each error/warning.
enum class ErrorDetailLevel : uint8_t {
	// [E/W<code>] <error name> at <line>:<position>
	ERROR_NAME_AND_CODE_ONLY = 0,

	// [E/W<code>] <error name> at <line>:<position>
	// <line of code>
	WITH_CODE,

	// [E/W<code>] <error name> at <line>:<position>
	// <line of code>
	// <error description>
	WITH_DESCRIPTION,

	// [E/W<code>] <error name> at <line>:<position>
	// <line of code>
	// <error description>
	// <advice on error fixing>; <examples>
	WITH_EXPLANATION
};

// Enumeration of codes for errors.
namespace ErrorCode {
	enum {
		// Internal compiler errors
		CANNOT_OPEN_FILE_ERROR = 0,
		INVALID_UTF_IN_FILE,

		// Lexer errors
		UNEXPECTED_CHARACTER,
		UNEXPECTED_TOKEN,
	};
}

// Enumeration of codes for warnings.
namespace WarningCode {
	enum {

	};
}

class ErrorPrinter final {
private:
	// Used to emulate named arguments in error and warning functions invocation.
	struct ErrorStruct {
		int code;
		utf::String name;
		TextPosition selectionStart;
		uint64_t selectionLength;
		utf::String description;
		utf::String explanation;
	};

private:
	// Contains the source file text currently being compiled.
	inline static utf::StringView s_currentSource = "";

public:
	inline static bool printWarnings = true;
	inline static ErrorDetailLevel errorDetailLevel = ErrorDetailLevel::WITH_EXPLANATION;

public:
	static void error(const ErrorStruct& err);
	static void warning(const ErrorStruct& err);

	// Must be called on new file compilation start.
	static void setSource(utf::StringView source);

private:
	// Does the actual output for errors/warnings.
	static void printMessage(char messageType, const ErrorStruct& err);
};