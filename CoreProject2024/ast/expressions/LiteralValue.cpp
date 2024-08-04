// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LiteralValue.hpp"
#include <charconv>
#include "error/ErrorPrinter.hpp"

ast::LiteralValue::LiteralValue(utf::StringView value) noexcept 
	: Expression(NodeType::LITERAL_VALUE), m_value(value) { }

utf::StringView ast::LiteralValue::getValue() const noexcept {
	return m_value;
}

int64_t ast::LiteralValue::parseAsI64() const noexcept {
	if (m_value.size() >= 2) {
		int64_t result = 0;
		switch (m_value[1]) {
			case 'x': return tryParseAsI64(16);
			case 'o': return tryParseAsI64(8);
			case 'b': return tryParseAsI64(2);
		default: break;
		}
	}

	return tryParseAsI64(10);
}

int64_t ast::LiteralValue::tryParseAsI64(int base) const noexcept {
	const char* first = m_value.data() + 2 * (base != 10);
	const char* end = m_value.data() + m_value.size();
	int64_t result = 0;
	auto convertionResult = std::from_chars(first, end, result, base);
	if (convertionResult.ec == std::errc::invalid_argument) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INVALID_NUMBER_LITERAL,
			.name = "Syntax error: Invalid number literal",
			.selectionStart = m_position,
			.selectionLength = m_value.size(),
			.description = "Cannot convert the number literal to a number: invalid argument.",
			.explanation = "-"
		});
	} else if (convertionResult.ec == std::errc::result_out_of_range) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INVALID_NUMBER_LITERAL,
			.name = "Syntax error: Invalid number literal",
			.selectionStart = m_position,
			.selectionLength = m_value.size(),
			.description = "Cannot convert the number literal to a number: result out of range.",
			.explanation = "-"
		});
	}

	return result;
}
