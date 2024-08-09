// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "IndentPrinter.hpp"
#include "error/ErrorPrinter.hpp"

utils::IndentPrinter::IndentPrinter(std::ostream& stream, utf::String indentString) noexcept
	: m_stream(stream), m_indentString(std::move(indentString)) { }

void utils::IndentPrinter::increaseIndent() {
	m_currentIndentation += m_indentString;
}

void utils::IndentPrinter::decreaseIndent() {
	if (m_currentIndentation.empty()) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INTERNAL_ERROR,
			.name = "Syntax error: Unexpected token",
			.description = "Tried to decrease indentation when it already was zero in IndentPrinter.",
		});

		return;
	}

	for (char _ : m_indentString) {
		m_currentIndentation.pop_back();
	}
}

void utils::IndentPrinter::printIndent() {
	m_stream << m_currentIndentation;
}

std::ostream& utils::IndentPrinter::stream() noexcept {
	return m_stream;
}
