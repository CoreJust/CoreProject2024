// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Symbol.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Symbol class that represents a single symbol.
*/

#pragma once
#include "SymbolPath.hpp"

namespace symbol {
	using SymbolId = uint64_t;

	enum SymbolKind : uint8_t {
		VARIABLE = 0,
		FUNCTION,

		SYMBOL_KIND_COUNT
	};

	class Symbol {
	protected:
		SymbolPath m_path;
		utf::String m_name;
		SymbolId m_id;
		SymbolKind m_kind;

	public:
		Symbol(SymbolPath path, utf::String name, SymbolKind kind) noexcept;
		inline virtual ~Symbol() { }

		const SymbolPath& getSymbolPath() const noexcept;
		const utf::String& getName() const noexcept;
		SymbolId getId() const noexcept;
		SymbolKind getKind() const noexcept;

		virtual utf::String toString() const noexcept = 0;

	private:
		// Generates a unique SymbolId on each call.
		// Thread-safe.
		static SymbolId generateUniqueId() noexcept;
	};
}