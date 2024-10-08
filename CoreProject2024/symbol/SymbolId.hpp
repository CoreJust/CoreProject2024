// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SymbolId.hpp by CoreJust
*	Created on 06.08.2024
*	Contains the SymbolId type that represents an ID of a symbol.
*/

#pragma once

namespace symbol {
	using SymbolId = unsigned long long;

	static_assert(sizeof(SymbolId) == 8);
}