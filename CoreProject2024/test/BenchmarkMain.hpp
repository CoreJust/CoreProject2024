// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	BenchmarkMain.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the benchmarks and BENCHMARK_MAIN().
*	Cannot coexist with the other main functions.
*/

#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include <llvm/ADT/SmallVector.h>
#include "utf/String.hpp"

uint8_t COUNTS[] = {
	1, 0, 1, 2, 3, 1, 2, 1, 4, 5, 8, 0, 1, 5, 1, 5, 4, 10, 11, 3, 2, 4, 2, 4, 6, 7, 2, 2, 1, 3, 2, 15, 1, 1, 2, 3, 2, 3, 4, 4, 4, 5, 29
};

static void BM_StdVector(benchmark::State& state) {
	srand(42);
	for (auto _ : state) {
		int cnt = rand() % std::size(COUNTS);
		std::vector<utf::String> vec;
		for (int i = 0; i < cnt; i++) {
			benchmark::DoNotOptimize(vec.emplace_back());
		}
	}
}
BENCHMARK(BM_StdVector);

static void BM_DefaultSmallVector(benchmark::State& state) {
	srand(42);
	for (auto _ : state) {
		int cnt = rand() % std::size(COUNTS);
		std::vector<utf::String> vec;
		for (int i = 0; i < cnt; i++) {
			benchmark::DoNotOptimize(vec.emplace_back());
		}
	}
}
BENCHMARK(BM_DefaultSmallVector);

static void BM_LargerSmallVector(benchmark::State& state) {
	srand(42);
	for (auto _ : state) {
		int cnt = rand() % std::size(COUNTS);
		std::vector<utf::String, 5> vec;
		for (int i = 0; i < cnt; i++) {
			benchmark::DoNotOptimize(vec.emplace_back());
		}
	}
}
BENCHMARK(BM_LargerSmallVector);

BENCHMARK_MAIN();