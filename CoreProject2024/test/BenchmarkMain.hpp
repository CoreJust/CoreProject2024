// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	BenchmarkMain.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the benchmarks and BENCHMARK_MAIN().
*	Cannot coexist with the other main functions.
*/

#include <benchmark/benchmark.h>
#include "compiler/Compiler.hpp"

static void BM_compilation(benchmark::State& state) {
	for (auto _ : state) {
		compiler::Compiler compiler;
		compiler.build();
	}
}
BENCHMARK(BM_compilation)->Iterations(10)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();