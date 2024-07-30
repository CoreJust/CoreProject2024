// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	main.cpp by CoreJust
*	Created on 18.07.2024
*	The main source file of the program that contains the entry point (function main()).
*/

#if 1
#include <iostream>
#include "utf/UtfIO.hpp"
#include "utils/File.hpp"
#include "error/ErrorPrinter.hpp"
#include "lexer/Tokenizer.hpp"
#include "ast/AstClassImplementations.hpp"
#include "ast/AstAllocator.hpp"
#include "ast_visitor/AstPrinter.hpp"

// The entry point of the CoreProject2024.
int main() {
	// Test tokenization.
	if (int code = utf::initUtfIO(); code != 0) {
		exit(code);
	}

	auto program = utils::readFile("test.core");
	if (!program.has_value()) {
		exit(1);
	}

	error::ErrorPrinter::setSource(*program);

	auto tokenizer = lexer::Tokenizer(*program);
	lexer::Token token = tokenizer.current();
	while (!token.isNoToken()) {
		std::cout << token.toString() << std::endl;
		token = tokenizer.next();
	}

	ast::Expression* val = ast::AstAllocator::make<ast::LiteralValue>("10");
	ast::Expression* ident = ast::AstAllocator::make<ast::IdentifierValue>("putchar");
	val = ast::AstAllocator::make<ast::InvocationOperator>(ident, std::vector<ast::Expression*>{ val });
	ast::Statement* putcharStatement = ast::AstAllocator::make<ast::ExpressionStatement>(val);

	val = ast::AstAllocator::make<ast::LiteralValue>("0");
	ast::Statement* retStatement = ast::AstAllocator::make<ast::ReturnStatement>(val);
	ast::Statement* block = ast::AstAllocator::make<ast::ScopeStatement>(std::vector<ast::Statement*>{ putcharStatement, retStatement });

	std::vector<ast::FunctionDeclaration::Argument> mainArgs { ast::FunctionDeclaration::Argument { .name = "arg", .type = "i32" } };
	ast::Declaration* mainFunc = ast::AstAllocator::make<ast::FunctionDeclaration>("main", "i32", std::move(mainArgs), block);

	utils::IndentPrinter printer(std::cout);
	ast_visitor::AstPrinter astPrinter(printer);
	astPrinter.print(mainFunc);

	ast::AstAllocator::clear();

	return 0;
}

#else // Code for benchmarking

#include <random>
#include <memory_resource>
#include <memory>
#include <vector>
#include <array>
#include <benchmark/benchmark.h>

static void BM_TrivialNew(benchmark::State& state) {
	for (auto _ : state) {
		switch (rand() % 5) {
			case 0: benchmark::DoNotOptimize(std::make_unique<uint8_t>()); break;
			case 1: benchmark::DoNotOptimize(std::make_unique<uint64_t>()); break;
			case 3: benchmark::DoNotOptimize(std::make_unique<std::string>()); break;
			case 4: benchmark::DoNotOptimize(std::make_unique<uint16_t>()); break;
		default: break;
		}
	}
}
BENCHMARK(BM_TrivialNew);

template<class T>
struct PMRDestructor {
	std::pmr::polymorphic_allocator<>& alloc;

	void operator()(T* ptr) const noexcept {
		alloc.deallocate(reinterpret_cast<std::byte*>(ptr), sizeof T);
	}
};

template<class T>
std::unique_ptr<T, PMRDestructor<T>> make_pmr_unique(std::pmr::polymorphic_allocator<>& alloc) {
	auto ptr = reinterpret_cast<T*>(alloc.allocate(sizeof(T)));
	::new(ptr) T();

	return std::unique_ptr<T, PMRDestructor<T>>(ptr, PMRDestructor<T> { .alloc = alloc });
}

static void BM_PooledPMR(benchmark::State& state) {
	std::pmr::unsynchronized_pool_resource res;
	std::pmr::polymorphic_allocator<> alloc { &res };
	
	for (auto _ : state) {
		switch (rand() % 5) {
			case 0: benchmark::DoNotOptimize(make_pmr_unique<uint8_t>(alloc)); break;
			case 1: benchmark::DoNotOptimize(make_pmr_unique<uint64_t>(alloc)); break;
			case 3: benchmark::DoNotOptimize(make_pmr_unique<std::string>(alloc)); break;
			case 4: benchmark::DoNotOptimize(make_pmr_unique<uint16_t>(alloc)); break;
		default: break;
		}
	}
}
BENCHMARK(BM_PooledPMR);

BENCHMARK_MAIN();
#endif