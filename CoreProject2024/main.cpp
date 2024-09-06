// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	main.cpp by CoreJust
*	Created on 18.07.2024
*	The main source file of the program that contains the entry point (function main()).
*/

/*
*	Core compilation pipeline (not completely implemented yet):
*	1) The source code file is read into a String.
*	=> String of source code
* 
*	2) A Tokenizer is created and gets the source code string.
*	   The Tokenizer can get Tokens one by one from the source code.
*	=> Token stream
* 
*	3) A Parser is created and gets the Tokenizer object.
*	   The Parser generates the Abstact Syntax Tree of the source code.
*	=> Abstract Syntax Tree (AST)
* 
*	4) The AST is traversed by AST SymbolLoader.
*	   It loads the information on symbol and fills up the symbol table.
*	=> Symbol Table (used only for symbol resolution)
* 
*	5) The Core High-level IR Generator generates the CHIR based on AST.
*	   All the symbols are recognized on this stage.
*	   Some language structs are simplified at this stage, explicit metadata is added.
*	   Type checks are done, as well as type inquires.
*	=> Core High-level IR (technically, it is more like another form of AST)
* 
*	6) The Compile-time Interpreter traverses the CHIR and does template instantiations
*	   and ct functions computations.
*	=> Core High-level IR without metacode
* 
*	7) The CIR Generator generates the Core IR from the CHIR. It is a more low-level 
*	   IR with maximal explicity, close to LLVM IR.
*	=> Core IR
* 
*	8) The CIR is traversed by CIR Passes that do some optimizations and checks.
*	=> Optimized Core IR.
* 
*	9) The LLVM Generator converts the Core IR into LLVM IR.
*	=> LLVM IR
* 
*	10) LLVM generates the object files, thus finishing compilation.
*	=> Machine code
*/

#define CORE_BUILD_MODE 0
#define CORE_BENCHMARKING_MODE 1

// By changing this macro value one can run current benchmarks, or build the actual project.
#define CURRENT_COMPILATION_MODE 1

#if CURRENT_COMPILATION_MODE == CORE_BUILD_MODE
#include <chrono>
#include <iostream>
#include "utf/UtfIO.hpp"
#include "compiler/Compiler.hpp"
#include "test/SourceCodeGenerator.hpp"

// The entry point of the CoreProject2024.
int main() {
	// Initializing Utf I/O.
	if (int code = utf::initUtfIO(); code != 0) {
		exit(code);
	}

	// Running the build.
	auto start = std::chrono::high_resolution_clock::now();
	compiler::Compiler compiler;
	compiler.build();

	auto compilationTime = std::chrono::high_resolution_clock::now() - start;
	std::cout << "\nCompilation time: " << (double(compilationTime.count()) / 1'000'000'000) << std::endl;

	return 0;
}

#elif CURRENT_COMPILATION_MODE == CORE_BENCHMARKING_MODE // Code for benchmarking
#include "test/BenchmarkMain.hpp" // Contains own main function
#endif