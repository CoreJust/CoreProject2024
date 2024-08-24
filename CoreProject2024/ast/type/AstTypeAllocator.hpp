// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstTypeAllocator.hpp by CoreJust
*	Created on 18.08.2024
*	Contains the AstTypeAllocator class that incapsulates a pool allocator for AST types allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "utils/TextPosition.hpp"
#include "AstType.hpp"

namespace ast {
	using AstTypeAllocator = utils::PooledAllocator<Type>;
}