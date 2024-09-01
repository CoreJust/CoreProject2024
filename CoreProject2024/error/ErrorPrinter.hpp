// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ErrorPrinter.hpp by CoreJust
*	Created on 25.07.2024
*	Contains the ErrorPrinter class that allows convenient error output based on the current
*	settings.
*/

#pragma once
#include "utf/String.hpp"
#include "utils/TextPosition.hpp"

namespace error {
	// ErrorDetailLevel specifies the amount of information printed for each error/warning.
	enum class ErrorDetailLevel : unsigned char {
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
			INTERNAL_ERROR, // For all the other internal errors

			// Project settings errors
			BAD_TARGET,

			// Lexer errors
			UNEXPECTED_CHARACTER,
			UNEXPECTED_TOKEN,
			NO_CLOSING_QUOTES,
			NO_COMMENT_END,
			NO_NUMBER_AFTER_NUMERIC_SYSTEM,

			// Parser errors
			EXPECTED_A_TYPE,
			EXPECTED_A_DECLARATION,
			EXPECTED_A_STATEMENT,
			NO_CLOSING_BRACE,

			// CHIR errors
			INVALID_NUMBER_LITERAL,
			UNRESOLVED_SYMBOL,
			TOO_MANY_SYMBOLS,
			INVALID_CALLEE,
			UNEXPECTED_TYPE,
			INVALID_TYPE_CONVERTION,
			TYPE_MISMATCH,
			BOOL_TYPE_REQUIRED,
			NON_FUNCTION_CONTEXT,
			RETURN_IN_RETURN,
			NO_MAIN_FUNCTION,
			IMPOSIBLE_INT_LITERAL,

			// CIR errors
			INSTRUCTION_AFTER_TERMINATOR,
			VERIFICATION_FAILED,

			// LLVM errors
			OBJECT_FILE_OPEN_FAILURE,

			ERROR_CODES_COUNT
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
			int code = -1;
			utf::String name = "-";
			utils::TextPosition selectionStart = utils::TextPosition { };
			uint64_t selectionLength = 0;
			utf::String description = "-";
			utf::String explanation = "-";
		};

	private:
		// Contains the source file text currently being compiled.
		inline static thread_local utf::String s_currentSource = "";

		// Set to true upon any error.
		inline static thread_local bool s_hasErrors = false;

	public:
		inline static bool printWarnings = true;
		inline static ErrorDetailLevel errorDetailLevel = ErrorDetailLevel::WITH_EXPLANATION;

	public:
		// Prints the error to the standard output stream.
		// Terminates the compilation process.
		[[noreturn]] static void fatalError(const ErrorStruct& err);

		// Prints the error to the standard output stream.
		static void error(const ErrorStruct& err);

		// Prints the warning to the standard output stream.
		static void warning(const ErrorStruct& err);

		// Sets the currently compiled source string, which is used to print source code lines on errors.
		// Must be called on new file compilation start.
		static utf::StringView setSource(utf::String source) noexcept;

		// Returns true if there were any errors.
		static bool hasErrors() noexcept;

	private:
		// Does the actual output for errors/warnings.
		static void printMessage(char messageType, const ErrorStruct& err);
	};
}