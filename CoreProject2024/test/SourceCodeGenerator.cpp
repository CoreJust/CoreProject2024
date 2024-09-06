// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SourceCodeGenerator.hpp"

uint32_t genRandomAgrumentsCount() {
	uint32_t argumentsCount = rand() % 12;
	switch (argumentsCount) {
		case 0: break;
		case 1: argumentsCount = 0; break;
		case 2: argumentsCount = 1; break;
		case 3: argumentsCount = 1; break;
		case 4: argumentsCount = 1; break;
		case 5: argumentsCount = 2; break;
		case 6: argumentsCount = 2; break;
		case 7: argumentsCount = 2; break;
		case 8: argumentsCount = 3; break;
		case 9: argumentsCount = 3 + (rand() % 2); break; // 3 and 4
		case 10: argumentsCount = 5 + (rand() % 3); break; // 5, 6 and 7
		case 11: argumentsCount = 8 + (rand() % 8); break; // 8-15
	default: break;
	}

	return argumentsCount;
}

test::SourceCodeGenerator::SourceCodeGenerator(uint32_t functionsCount, uint32_t maxExpressionsPerFunctionCount, uint32_t maxExpressionDepth) noexcept
	: m_functionsCount(functionsCount), m_maxExpressionsPerFunctionCount(maxExpressionsPerFunctionCount), m_maxExpressionDepth(maxExpressionDepth) { }

void test::SourceCodeGenerator::generateSourceFile(const utf::String& fileName) {
	m_file.open(fileName, std::ios::out | std::ios::trunc);
	m_file << "fn putchar(ch: cint): cint = native(\"putchar\")\n\n";
	m_file << "fn eval0(a: i64): i64 = putchar(a as cint) as i64\n\n";

	srand(42);
	for (uint32_t i = 0; i < m_functionsCount; ++i) {
		generateFunction(i);
	}

	m_file << "fn main(): cint {\n\treturn 0\n}";
	m_file.flush();
	m_file.close();
}

void test::SourceCodeGenerator::generateFunction(uint32_t index) {
	bool isPrintFunction = rand() % 5 == 0;
	if (isPrintFunction) {
		generatePrintFunction(m_printFunctionsCount++);
	} else { // Eval function
		uint32_t argumentsCount = genRandomAgrumentsCount();
		generateEvalFunction(m_evalFunctionsCount[argumentsCount]++, argumentsCount);
	}
}

void test::SourceCodeGenerator::generatePrintFunction(uint32_t index) {
	m_file << "fn print" << index << "(x: i64) {\n";
	m_file << "\tif x > " << (rand() % 2048) << " {\n";
	generatePutcharSequence(rand() % m_maxExpressionDepth);
	m_file << "\t} else {\n";
	generatePutcharSequence(rand() % m_maxExpressionDepth);
	m_file << "\t}\n}\n\n";
}

void test::SourceCodeGenerator::generateEvalFunction(uint32_t index, uint32_t argumentsCount) {
	m_file << "fn eval" << index << '(';
	if (argumentsCount) {
		m_file << "x0: i64";
	}

	for (uint32_t i = 1; i < argumentsCount; ++i) {
		m_file << ", x" << i << ": i64";
	}

	m_file << "): i64 {";

	uint32_t expressionsCount = 1 + (rand() % m_maxExpressionsPerFunctionCount);
	for (uint32_t i = 0; i < expressionsCount; ++i) {
		uint32_t variableIndex = argumentsCount + i;
		m_file << "\n\tlet x" << variableIndex << ": i64 = ";
		generateExpression(variableIndex, rand() % m_maxExpressionDepth);
	}

	uint32_t lastVariableIndex = expressionsCount + argumentsCount - 1;
	if (m_printFunctionsCount) {
		m_file << "\n\tprint" << (rand() % m_printFunctionsCount) << "(x" << lastVariableIndex << ")";
	}

	m_file << "\n\treturn x" << lastVariableIndex << "\n}\n\n";
}

void test::SourceCodeGenerator::generatePutcharSequence(uint32_t size) {
	while (size--) {
		m_file << "\t\tputchar(" << (32 + rand() % 96) << ")\n";
	}
}

void test::SourceCodeGenerator::generateExpression(uint32_t index, uint32_t depth) {
	if (depth == 1 && rand() % 2 == 0) {
		uint32_t argumentsCount = genRandomAgrumentsCount();
		while (m_evalFunctionsCount[argumentsCount] == 0) {
			if (argumentsCount == 0) {
				argumentsCount = 15;
			} else {
				argumentsCount -= 1;
			}
		}

		uint32_t calleeIndex = rand() % m_evalFunctionsCount[argumentsCount];
		m_file << "eval" << calleeIndex << '(';
		if (argumentsCount--) {
			generateExpression(index, 0);

			while (argumentsCount--) {
				m_file << ", ";
				generateExpression(index, 0);
			}
		}

		m_file << ')';
		return;
	}

	if (depth <= 0) {
		if (rand() % 2 == 0 || index == 0) {
			m_file << (rand() % 1024);
		} else {
			m_file << 'x' << (rand() % index);
		}

		return;
	}

	if (rand() % 4 == 0) { // Unary
		if (depth > 1) {
			m_file << "-(";
			generateExpression(index, depth - 1);
			m_file << ")";
		} else {
			m_file << "-";
			generateExpression(index, depth - 1);
		}
	} else { // Binary
		generateExpression(index, depth - 1);
		char operatorCh = "+-*/%"[rand() % 5];
		m_file << " " << operatorCh << " ";
		if (operatorCh == '/' || operatorCh == '%') { // So as to avoid division by zero.
			m_file << "(1 + ";
			generateExpression(index, depth - 1);
			m_file << ')';
		} else {
			generateExpression(index, depth - 1);
		}
	}
}
