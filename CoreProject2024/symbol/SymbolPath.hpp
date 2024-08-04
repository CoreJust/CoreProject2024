// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SymbolPath.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the SymbolPath class that represents a path to the symbol,
*	e.g. std.core.mem for a symbol in file <std project>/core/mem.core.
*/

#pragma once
#include <vector>
#include "utf/String.hpp"

namespace symbol {
	// Basic struct for a path, which can represent not only symbol path.
	struct Path final {
		std::vector<utf::String> path;

		utf::String toString() const noexcept;
	};

	struct SymbolPath final {
		// The path within module, e.g. Vec.Iter for symbol in Iter class in Vec class.
		Path internalPath;
	};
}
