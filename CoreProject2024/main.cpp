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
#define CURRENT_COMPILATION_MODE CORE_BUILD_MODE

#if CURRENT_COMPILATION_MODE == CORE_BUILD_MODE
#include <iostream>
#include "utf/UtfIO.hpp"
#include "utils/File.hpp"
#include "error/ErrorPrinter.hpp"
#include "lexer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "symbol/SymbolAllocator.hpp"
#include "ast_visitor/AstPrinter.hpp"
#include "ast_visitor/SymbolLoader.hpp"
#include "ast_visitor/CHIRGenerator.hpp"
#include "ast/AstAllocator.hpp"
#include "chir/ChirAllocator.hpp"
#include "chir_visitor/CirGlobalsLoader.hpp"
#include "chir_visitor/CirGenerator.hpp"
#include "cir/CirModule.hpp"

constexpr char SOURCE_FILE[] = "test.core";
constexpr bool PRINT = true;

// The entry point of the CoreProject2024.
int main() {
	// Initializing Utf I/O.
	if (int code = utf::initUtfIO(); code != 0) {
		exit(code);
	}

	// Loading the source file.
	std::optional<utf::String> program = utils::readFile(SOURCE_FILE);
	if (!program) {
		exit(1);
	}

	// Setting the source file as error source.
	error::ErrorPrinter::setSource(*program);

	try {
		// Parsing the source code.
		ast::Declaration* programAst =
			parser::Parser(lexer::Tokenizer(*program))
			.parse();

		if (error::ErrorPrinter::hasErrors()) {
			throw 0;
		}

		// Printing the AST.
		utils::IndentPrinter printer(std::cout, "    ");
		ast_visitor::AstPrinter astPrinter(printer);

		if (PRINT) {
			std::cout << "AST:\n\n";
			astPrinter.print(programAst);
		}
		
		// Loading symbols of the code.
		ast_visitor::SymbolLoader symbolLoader(symbol::g_symbolTable);
		symbolLoader.loadSymbols(programAst);

		if (error::ErrorPrinter::hasErrors()) {
			throw 0;
		}

		// Generating CHIR.
		ast_visitor::CHIRGenerator chirGenerator(symbol::g_symbolTable);
		chir::Module chirModule = chirGenerator.generateCHIRModule(programAst);

		// It is safe to clear memory for AST now.
		ast::AstAllocator::tryRelease();

		if (error::ErrorPrinter::hasErrors()) {
			throw 0;
		}

		// Printing the CHIR
		if (PRINT) {
			std::cout << "\n\nCHIR:\n\n";
			chirModule.print(printer);
		}

		// Generating CIR.
		cir::Module cirModule(SOURCE_FILE);

		chir_visitor::CirGlobalsLoader globalsLoader(cirModule);
		globalsLoader.visitRoot(chirModule);

		chir_visitor::CirGenerator cirGenerator(cirModule, globalsLoader.getGlobalsMap());
		cirGenerator.visitRoot(chirModule);

		chir::ChirAllocator::tryRelease();
		symbol::SymbolAllocator::tryRelease();

		// Printing the CIR
		if (PRINT) {
			std::cout << "\n\nCIR:\n\n";
			cirModule.print(std::cout);
		}
	} catch (...) {
		std::cerr << "Build failed!\n";
	}

	ast::AstAllocator::tryRelease();
	chir::ChirAllocator::tryRelease();
	symbol::SymbolAllocator::tryRelease();
	cir::CirAllocator::tryRelease();

	return 0;
}

#elif CURRENT_COMPILATION_MODE == CORE_BENCHMARKING_MODE // Code for benchmarking
#include "test/BenchmarkMain.hpp" // Contains own main function
#endif