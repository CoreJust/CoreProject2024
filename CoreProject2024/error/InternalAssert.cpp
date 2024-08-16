// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "InternalAssert.hpp"
#include <format>
#include "ErrorPrinter.hpp"

void error::internalAssert(bool condition, utf::StringView message, const std::source_location& location) {
	if (!condition) {
		ErrorPrinter::fatalError({
			.code = ErrorCode::INTERNAL_ERROR,
			.name = "Internal error: Internal assertion failure",
			.description = std::format(
				"{}, source: {} at {}:{}, function {}.",
				message,
				location.file_name(),
				location.line(),
				location.column(),
				location.function_name()
			)
		});
	}
}
