// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirAllocator.hpp by CoreJust
*	Created on 05.08.2024
*	Contains the CirAllocator class that incapsulates a pool allocator for CIR values.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "value/CirValue.hpp"

namespace cir {
	using CirAllocator = utils::PooledAllocator<Value>;
}