// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SourceCodeGenerator.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the SourceCodeGenerator class that generates core source files for other tests.
*/

#pragma once
#include <cstdlib>
#include <fstream>
#include "utf/String.hpp"

namespace test {
	/*
	*	Generates a single source code file with a large (specified) number of functions.
	*/
	class SourceCodeGenerator final {
	private:
		uint32_t m_functionsCount;
		uint32_t m_maxExpressionsPerFunctionCount;
		uint32_t m_maxExpressionDepth;
		std::ofstream m_file;

	public:
		SourceCodeGenerator(uint32_t functionsCount, uint32_t maxExpressionsPerFunctionCount, uint32_t maxExpressionDepth) noexcept;

		void generateSourceFile(const utf::String& fileName);

	private:
		void generateFunction(uint32_t index);
		void generateExpression(uint32_t index, uint32_t depth);
	};
}