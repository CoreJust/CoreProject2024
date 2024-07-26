// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	main.cpp by CoreJust
*	Created on 18.07.2024
*	The main source file of the program that contains the entry point (function main()).
*/

#if 1
#include <iostream>
#include "utils/UtfIO.hpp"
#include "utils/File.hpp"
#include "utils/ErrorPrinter.hpp"
#include "lexer/Tokenizer.hpp"

// The entry point of the CoreProject2024.
int main() {
	// Test tokenization.
	if (int code = utf::initUtfIO(); code != 0) {
		exit(code);
	}

	auto program = file::readFile("test.core");
	if (!program.has_value()) {
		exit(1);
	}

	ErrorPrinter::setSource(*program);

	auto tokenizer = Tokenizer(*program);
	auto token = tokenizer.current();
	while (!token.isNoToken()) {
		std::cout << token.toString() << std::endl;
		token = tokenizer.next();
	}

	return 0;
}

#else // Code for benchmarking

#include <random>
#include <fstream>
#include <filesystem>
#include <benchmark/benchmark.h>
#include "utils/File.hpp"

constexpr auto TEST_FILE = "text.core";
constexpr size_t TEST_FILE_SIZE = 0x1000000;

static void BM_LargeTextFileSetup(const benchmark::State& state) {
	static std::string CHARS = " \t\n\r_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distr(0, CHARS.size());

	std::ofstream file(TEST_FILE, std::ios::out | std::ios::trunc);
	for (size_t i = 0; i < TEST_FILE_SIZE; i++) {
		file << CHARS[distr(gen)];
	}
}

static void BM_LargeTextFileTeardown(const benchmark::State& state) {
	std::filesystem::remove(TEST_FILE);
}

static void BM_TrivialFileReading(benchmark::State& state) {
	for (auto _ : state) {
		benchmark::DoNotOptimize(file::readFileUnchecked(TEST_FILE));
	}
}
BENCHMARK(BM_TrivialFileReading)->Unit(benchmark::kMillisecond)->Iterations(5)->Setup(BM_LargeTextFileSetup)->Teardown(BM_LargeTextFileTeardown);

static void BM_FileReadingWithReserve(benchmark::State& state) {
	for (auto _ : state) {
		benchmark::DoNotOptimize(file::readFileUnchecked_reservingBuffer(TEST_FILE));
	}
}
BENCHMARK(BM_FileReadingWithReserve)->Unit(benchmark::kMillisecond)->Iterations(5)->Setup(BM_LargeTextFileSetup)->Teardown(BM_LargeTextFileTeardown);

static void BM_FileReadingWithOnwBuffer(benchmark::State& state) {
	for (auto _ : state) {
		benchmark::DoNotOptimize(file::readFileUnchecked_ownBuffer(TEST_FILE));
	}
}
BENCHMARK(BM_FileReadingWithOnwBuffer)->Unit(benchmark::kMillisecond)->Iterations(5)->Setup(BM_LargeTextFileSetup)->Teardown(BM_LargeTextFileTeardown);

static void BM_FileReadingWithDirectStorageCpp(benchmark::State& state) {
	for (auto _ : state) {
		benchmark::DoNotOptimize(file::readFileUnchecked_directStorageCpp(TEST_FILE));
	}
}
BENCHMARK(BM_FileReadingWithDirectStorageCpp)->Unit(benchmark::kMillisecond)->Iterations(5)->Setup(BM_LargeTextFileSetup)->Teardown(BM_LargeTextFileTeardown);

static void BM_FileReadingWithDirectStorageC(benchmark::State& state) {
	for (auto _ : state) {
		benchmark::DoNotOptimize(file::readFileUnchecked_directStorageC(TEST_FILE));
	}
}
BENCHMARK(BM_FileReadingWithDirectStorageC)->Unit(benchmark::kMillisecond)->Iterations(5)->Setup(BM_LargeTextFileSetup)->Teardown(BM_LargeTextFileTeardown);

BENCHMARK_MAIN();
#endif