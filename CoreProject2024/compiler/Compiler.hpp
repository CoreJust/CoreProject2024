// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Compiler.hpp by CoreJust
*	Created on 09.08.2024
*	Contains Compiler class that handles the compilation pipeline.
*/

#pragma once
#include <llvm/Target/TargetMachine.h>
#include "utf/String.hpp"
#include "ast/Declaration.hpp"
#include "symbol/SymbolTable.hpp"
#include "chir/ChirModule.hpp"
#include "cir/CirModule.hpp"
#include "llvm_utils/LLVMModule.hpp"

namespace compiler {
	class Compiler final {
	public:
		Compiler();

		// Does the actual build of the Core project.
		void build();

	private:
		// Opens the source code file and loads its contents.
		// Handles the errors by throwing 0.
		utf::StringView loadModule();

		// Parses the source code string into AST.
		utils::NoNull<ast::Declaration> parseModule(utf::StringView sourceCode);

		// Loads the symbols from AST.
		std::unique_ptr<symbol::SymbolTable> loadSymbols(utils::NoNull<ast::Declaration> ast);

		// Generates the Core High-level IR based on AST.
		std::unique_ptr<chir::Module> generateCHIR(utils::NoNull<ast::Declaration> ast, std::unique_ptr<symbol::SymbolTable> symbolTable);

		// Generates the Core IR based on Core High-level IR.
		std::unique_ptr<cir::Module> generateCIR(std::unique_ptr<chir::Module> chirModule);

		// Generates the LLVM IR based on Core IR.
		llvm::TargetMachine* generateLLVM(std::unique_ptr<cir::Module> cirModule, llvm_utils::LLVMModule& result);

		// Creates the object code file based on LLVM IR.
		void generateObjectFile(llvm_utils::LLVMModule&& llvmModule, llvm::TargetMachine* targetMachine);

		// Links the object files.
		void link();
		
		// Runs the compiled project.
		void execute();

		// Checks if there were errors on the previos compilation phase
		// and throws if there were.
		void checkForErrors();

		void cleanup();
	};
}