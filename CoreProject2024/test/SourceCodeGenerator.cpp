// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SourceCodeGenerator.hpp"

test::SourceCodeGenerator::SourceCodeGenerator(uint32_t functionsCount, uint32_t maxExpressionsPerFunctionCount, uint32_t maxExpressionDepth) noexcept
	: m_functionsCount(functionsCount), m_maxExpressionsPerFunctionCount(maxExpressionsPerFunctionCount), m_maxExpressionDepth(maxExpressionDepth) { }

void test::SourceCodeGenerator::generateSourceFile(const utf::String& fileName) {
	m_file.open(fileName, std::ios::out | std::ios::trunc);

	srand(42);
	for (uint32_t i = 0; i < 5000; i++) {
		generateFunction(i);
	}
}

void test::SourceCodeGenerator::generateFunction(uint32_t index) {
	m_file << "fn f" << index << "(a: i32, b: i32): i32 {\n";

	uint32_t count = 1 + rand() % 32;
	for (uint32_t i = 0; i < count; i++) {
		m_file << "\tlet x" << i << ": i32 = ";
		generateExpression(i, 1 + rand() % 6);
		m_file << '\n';
	}

	m_file << "\treturn x" << (count - 1) << "\n";
	m_file << "}\n\n";
}

void test::SourceCodeGenerator::generateExpression(uint32_t index, uint32_t depth) {
	if (depth <= 0) {
		if (rand() % 2 == 0 || index == 0) {
			m_file << rand();
		} else {
			m_file << 'x' << (rand() % index);
		}

		return;
	}

	if (rand() % 3 == 0) { // Unary
		m_file << "-(";
		generateExpression(index, depth - 1);
		m_file << ')';
	} else { // Binary
		m_file << '(';
		generateExpression(index, depth - 1);
		m_file << ") " << "+-*/%"[rand() % 5] << " (";
		generateExpression(index, depth - 1);
		m_file << ')';
	}
}
