// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// TODO: refactor
#include "Compiler.hpp"
#include <iostream>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/LegacyPassManager.h>
#include "CompilerOptions.hpp"
#include "utils/File.hpp"
#include "error/ErrorPrinter.hpp"
#include "parser/Parser.hpp"
#include "symbol/SymbolAllocator.hpp"
#include "symbol/type/TypeAllocator.hpp"
#include "ast_visitor/AstPrinter.hpp"
#include "ast_visitor/SymbolLoader.hpp"
#include "ast_visitor/CHIRGenerator.hpp"
#include "ast/AstAllocator.hpp"
#include "ast/type/AstTypeAllocator.hpp"
#include "chir/ChirAllocator.hpp"
#include "chir_visitor/CirGlobalsLoader.hpp"
#include "chir_visitor/CirGenerator.hpp"
#include "cir/type/CirTypeAllocator.hpp"
#include "cir_pass/CirPassManager.hpp"
#include "cir_pass/optimization/CirDeadInstructionsEliminatorPass.hpp"
#include "cir_pass/CirVerificationPass.hpp"
#include "cir_pass/LLVMGlobalsLoaderPass.hpp"
#include "cir_pass/LLVMGenerator.hpp"
#include "llvm_utils/LLVMModule.hpp"

compiler::Compiler::Compiler() { }

void compiler::Compiler::build() {
	try {
		utf::StringView program = loadModule();
		ast::Declaration* ast = parseModule(program);

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::CHIR)) {
			cleanup();
			return;
		}

		symbol::SymbolTable symbolTable = loadSymbols(ast);
		chir::Module chirModule = generateCHIR(ast, std::move(symbolTable));
		ast::AstAllocator::tryRelease();
		ast::AstTypeAllocator::tryRelease();

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::CIR)) {
			cleanup();
			return;
		}

		cir::Module cirModule = generateCIR(std::move(chirModule));
		chir::ChirAllocator::tryRelease();
		symbol::SymbolAllocator::tryRelease();
		symbol::TypeAllocator::tryRelease();

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::LLVM_IR)) {
			cleanup();
			return;
		}

		llvm_utils::LLVMModule llvmModule { CompilerOptions::getSourceName() };
		llvm::TargetMachine* targetMachine = generateLLVM(std::move(cirModule), llvmModule);
		cir::CirAllocator::tryRelease();
		cir::CirTypeAllocator::tryRelease();

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::OBJECT_FILE)) {
			cleanup();
			return;
		}

		generateObjectFile(std::move(llvmModule), targetMachine);

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::EXECUTABLE_FILE)) {
			cleanup();
			return;
		}

		link();

		if (!CompilerOptions::shallCompileUpToPhase(EmitMode::EXECUTION_RESULTS) || CompilerOptions::isLibraryMode()) {
			cleanup();
			return;
		}

		execute();
	} catch (...) {
		std::cerr << "Build failed!" << std::endl;

		cleanup();
	}
}

utf::StringView compiler::Compiler::loadModule() {
	std::optional<utf::String> program = utils::readFile(CompilerOptions::getSourceName() + ".core");
	checkForErrors();

	if (!program) {
		throw 0;
	}

	return error::ErrorPrinter::setSource(std::move(*program));
}

ast::Declaration* compiler::Compiler::parseModule(utf::StringView sourceCode) {
	ast::Declaration* result = parser::Parser(lexer::Tokenizer(sourceCode)).parse();
	checkForErrors();

	if (CompilerOptions::shallEmitAST()) {
		utils::IndentPrinter printer(std::cout, "	");
		ast_visitor::AstPrinter astPrinter(printer);

		std::cout << "AST:\n\n";
		astPrinter.print(result);
	}

	return result;
}

symbol::SymbolTable compiler::Compiler::loadSymbols(ast::Declaration* ast) {
	symbol::SymbolTable result;
	ast_visitor::SymbolLoader symbolLoader(result);

	symbolLoader.loadSymbols(ast);
	checkForErrors();

	// TMP: Check for main function
	if (CompilerOptions::isExecutableMode()) {
		const symbol::FunctionSymbol* mainFunction = result.getFunction("main", { });
		if (mainFunction == nullptr) {
			error::ErrorPrinter::fatalError({
				.code = error::ErrorCode::NO_MAIN_FUNCTION,
				.name = "Semantic error: No main function found",
				.description = "In executable project mode the main function is required, but none was found."
			});
		}
	}

	return result;
}

chir::Module compiler::Compiler::generateCHIR(ast::Declaration* ast, symbol::SymbolTable&& symbolTable) {
	ast_visitor::CHIRGenerator chirGenerator(symbolTable);
	chir::Module result = chirGenerator.generateCHIRModule(ast);
	checkForErrors();

	if (CompilerOptions::shallEmitCHIR()) {
		std::cout << "\n\nCHIR:\n\n";

		utils::IndentPrinter printer(std::cout, "	");
		result.print(printer);
	}

	return result;
}

cir::Module compiler::Compiler::generateCIR(chir::Module&& chirModule) {
	cir::Module result { CompilerOptions::getSourceName() };

	chir_visitor::CirGlobalsLoader globalsLoader(result);
	globalsLoader.visitRoot(chirModule);
	checkForErrors();

	chir_visitor::CirGenerator cirGenerator(result, globalsLoader.getGlobalsMap());
	cirGenerator.visitRoot(chirModule);
	checkForErrors();

	// Printing the CIR.
	if (CompilerOptions::shallEmitCIR()) {
		std::cout << "\n\nCIR:\n\n";
		result.print(std::cout);
	}

	// CIR passes.
	cir_pass::PassManager cirPassManager;

	// Obligatory passes.
	cirPassManager.registerPass(std::make_unique<cir_pass::DeadInstructionsEliminatorPass>());
	cirPassManager.registerPass(std::make_unique<cir_pass::VerificationPass>()); // CIR verification.

	cirPassManager.pass(&result);
	checkForErrors();

	// Printing the CIR after all the optimizations.
	if (CompilerOptions::shallEmitOptimizedCIR()) {
		std::cout << "\n\nOptimized CIR:\n\n";
		result.print(std::cout);
	}

	return result;
}

llvm::TargetMachine* compiler::Compiler::generateLLVM(cir::Module&& cirModule, llvm_utils::LLVMModule& result) {
	cir_pass::LLVMGlobalsLoaderPass llvmGlobalLoaderPass(result);

	llvmGlobalLoaderPass.pass(&cirModule);

	// Generating LLVM IR.
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();
	
	cir_pass::LLVMGenerator llvmGenerator(result, llvmGlobalLoaderPass.getLLVMGlobals());
	llvmGenerator.pass(&cirModule);

	// Printing LLVM IR.
	if (CompilerOptions::shallEmitLLVMIR()) {
		std::cout << "\n\nLLVM IR:\n\n";
		result.getModule().print(llvm::errs(), nullptr);
	}

	// Optimizing LLVM IR.
	llvmGenerator.optimize();

	// Printing optimized LLVM IR.
	if (CompilerOptions::shallEmitOptimizedLLVMIR()) {
		std::cout << "\n\nOptimized LLVM IR:\n\n";
		result.getModule().print(llvm::errs(), nullptr);
	}

	return llvmGenerator.getTargetMachine();
}

void compiler::Compiler::generateObjectFile(llvm_utils::LLVMModule&& llvmModule, llvm::TargetMachine* targetMachine) {
	// Opening the object file for writing.
	std::error_code objectCodeError;
	llvm::raw_fd_ostream objectCodeOutput(CompilerOptions::getSourceName() + ".o", objectCodeError, llvm::sys::fs::OF_None);
	if (objectCodeError) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::OBJECT_FILE_OPEN_FAILURE,
			.name = "Compilation error: Failed to open object file",
			.description = std::format("Failed to open {}.o: {}.", CompilerOptions::getSourceName(), objectCodeError.message()),

		});
	}

	// Creating PassManager for object code generation.
	llvm::legacy::PassManager passManager;
	if (targetMachine->addPassesToEmitFile(passManager, objectCodeOutput, nullptr, llvm::CodeGenFileType::ObjectFile)) {
		throw 0;
	}

	// Emitting the assembly.
	if (CompilerOptions::shallEmitAssembly()) {
		llvm::legacy::PassManager emitPassManager;
		if (targetMachine->addPassesToEmitFile(emitPassManager, llvm::errs(), nullptr, llvm::CodeGenFileType::AssemblyFile)) {
			throw 0;
		}

		std::cout << "\n\nAssembly:\n\n";
		emitPassManager.run(llvmModule.getModule());
		llvm::errs().flush();
	}

	// Generating the object code.
	passManager.run(llvmModule.getModule());
	objectCodeOutput.flush();
	objectCodeOutput.close();
}

void compiler::Compiler::link() {
	utf::String linker = utf::String(CompilerOptions::getLinker());
	utf::String linkCommand = std::format("{0} -o {1}.exe {1}.o", linker, CompilerOptions::getSourceName());

	system(linkCommand.c_str());
}

void compiler::Compiler::execute() {
	system((CompilerOptions::getSourceName() + ".exe").c_str());
	system("pause");
}

void compiler::Compiler::checkForErrors() {
	if (error::ErrorPrinter::hasErrors()) {
		throw 0; // Terminating compilation.
	}
}

void compiler::Compiler::cleanup() {
	ast::AstAllocator::tryRelease();
	ast::AstTypeAllocator::tryRelease();
	chir::ChirAllocator::tryRelease();
	symbol::SymbolAllocator::tryRelease();
	symbol::TypeAllocator::tryRelease();
	cir::CirAllocator::tryRelease();
	cir::CirTypeAllocator::tryRelease();
}
