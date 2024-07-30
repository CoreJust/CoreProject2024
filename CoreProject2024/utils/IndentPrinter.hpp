// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	IndentPrinter.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the IndentPrinter class that allows convenient printing with an indent.
*	Before each printed line it outputs the current indent, which can be increased / decreased.
*/

#pragma once
#include <ostream>
#include "utf/String.hpp"

namespace utils {
	class IndentPrinter final {
	private:
		std::ostream& m_stream;
		utf::String m_currentIndentation;
		utf::String m_indentString; // The characters used for a single indentation.

	public:
		IndentPrinter(std::ostream& stream, utf::String indentString = "\t") noexcept;

		void increaseIndent();
		void decreaseIndent();
		void printIndent();

		std::ostream& stream() noexcept;
	};
}