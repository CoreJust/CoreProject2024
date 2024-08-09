// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SymbolAllocator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the SymbolAllocator class that incapsulates a pool allocator for Symbol allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "Symbol.hpp"

namespace symbol {
	using SymbolAllocator = utils::PooledAllocator<Symbol>;
}