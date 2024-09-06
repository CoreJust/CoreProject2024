// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TypeAllocator.hpp by CoreJust
*	Created on 18.08.2024
*	Contains the TypeAllocator class that incapsulates a pool allocator for symbol types allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "utils/TextPosition.hpp"
#include "Type.hpp"

namespace symbol {
	using TypeAllocator = utils::PooledAllocator<Type>;
}