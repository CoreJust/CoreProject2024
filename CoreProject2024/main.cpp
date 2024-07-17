/*
*	main.cpp by CoreJust
*	Created on 18.07.2024
*	The main source file of the program that contains the entry point (function main()).
*/

#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

// The entry point of the CoreProject2024.
int main() {
	// Test contents to check if the LLVM was successfully added to the project.
	auto context = llvm::LLVMContext();
	auto test_module = llvm::Module("test_module", context);
	auto builder = llvm::IRBuilder<>(context);

	auto void_type = llvm::Type::getVoidTy(context);
	auto main_function_type = llvm::FunctionType::get(void_type, false);
	auto main_function = llvm::Function::Create(main_function_type, llvm::Function::ExternalLinkage, 0, "main", &test_module);

	auto basic_block = llvm::BasicBlock::Create(context, "main", main_function);

	builder.SetInsertPoint(basic_block);
	builder.CreateRetVoid();

	std::cout << "Test llvm module:" << std::endl;
	test_module.dump();

	return 0;
}