// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	BenchmarkMain.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the benchmarks and BENCHMARK_MAIN().
*	Cannot coexist with the other main functions.
*/

#include <map>
#include <benchmark/benchmark.h>
#include "utils/StringHash.hpp"

std::map<utf::StringView, int> TOKENS = {
	{ "ja", 10 },
	{ "nein", 1 },
	{ "+", 13 },
	{ "-", 63 },
	{ "function", 785 },
	{ "native", 3624 },
	{ "return", 672674 },
	{ "?", 375 },
	{ ">>>", 264 }
};

constexpr int getValue(utf::StringView str) noexcept {
	switch (utils::hashOf(str)) {
		case utils::hashOf("ja"): return 10;
		case utils::hashOf("nein"): return 1;
		case utils::hashOf("+"): return 13;
		case utils::hashOf("-"): return 63;
		case utils::hashOf("function"): return 785;
		case utils::hashOf("native"): return 3624;
		case utils::hashOf("return"): return 672674;
		case utils::hashOf("?"): return 375;
		case utils::hashOf(">>>"): return 264;
	default: return 0;
	}
}

static void BM_Map(benchmark::State& state) {
	utf::String ARR[] = { "ja", "nein", "+", "-", "function", "native", "return", "?", ">>>" };

	uint64_t i = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(TOKENS.find(ARR[(i += 5) % std::size(ARR)]));
	}
}
BENCHMARK(BM_Map);

static void BM_Switch(benchmark::State& state) {
	utf::String ARR[] = { "ja", "nein", "+", "-", "function", "native", "return", "?", ">>>" };

	uint64_t i = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(getValue(ARR[(i += 5) % std::size(ARR)]));
	}
}
BENCHMARK(BM_Switch);

BENCHMARK_MAIN();