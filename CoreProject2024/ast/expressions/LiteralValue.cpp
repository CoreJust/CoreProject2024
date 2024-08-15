// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LiteralValue.hpp"
#include <charconv>
#include "error/InternalAssert.hpp"
#include "error/ErrorPrinter.hpp"

// Values used for bool literals.
const utf::StringView BOOL_FALSE = "\0";
const utf::StringView BOOL_TRUE = "\x1";

ast::LiteralValue::LiteralValue(utf::StringView value) noexcept 
	: Expression(NodeKind::LITERAL_VALUE), m_value(value), m_type("i32") { }

ast::LiteralValue::LiteralValue(bool value) noexcept
	: Expression(NodeKind::LITERAL_VALUE), m_value(value ? BOOL_TRUE : BOOL_FALSE), m_type("bool") { }

const ast::Type& ast::LiteralValue::getType() const noexcept {
	return m_type;
}

utf::StringView ast::LiteralValue::getRawValue() const noexcept {
	return m_value;
}

int64_t ast::LiteralValue::parseAsI64() const noexcept {
	error::internalAssert(m_type.getTypeName() == "i32", "Must be i32");

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

bool ast::LiteralValue::parseAsBool() const noexcept {
	error::internalAssert(m_type.getTypeName() == "bool", "Must be bool");

	return m_value[0];
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
		});
	} else if (convertionResult.ec == std::errc::result_out_of_range) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INVALID_NUMBER_LITERAL,
			.name = "Syntax error: Invalid number literal",
			.selectionStart = m_position,
			.selectionLength = m_value.size(),
			.description = "Cannot convert the number literal to a number: result out of range.",
		});
	}

	return result;
}
