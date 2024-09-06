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

		uint32_t m_evalFunctionsCount[16] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // [number of arguments], fn eval<n>(args: i64...): i64
		uint32_t m_printFunctionsCount = 0; // fn print<n>(arg: i64)

	public:
		SourceCodeGenerator(uint32_t functionsCount = 5000, uint32_t maxExpressionsPerFunctionCount = 16, uint32_t maxExpressionDepth = 6) noexcept;

		void generateSourceFile(const utf::String& fileName);

	private:
		void generateFunction(uint32_t index);
		void generatePrintFunction(uint32_t index);
		void generateEvalFunction(uint32_t index, uint32_t argumentsCount);
		void generatePutcharSequence(uint32_t size);
		void generateExpression(uint32_t index, uint32_t depth);
	};
}