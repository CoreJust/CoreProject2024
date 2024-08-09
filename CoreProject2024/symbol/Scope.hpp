// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Scope.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Scope class that represents a scope in the symbol table.
*/

#pragma once
#include <vector>
#include <list>
#include "FunctionSymbol.hpp"

namespace symbol {
	using ScopeId = uint64_t;

	class Scope final {
	private:
		ScopeId m_id;
		Scope* m_parent;
		FunctionSymbol* m_function; // The function this scope belongs to.
		utf::String m_name;
		std::vector<utils::NoNull<Symbol>> m_symbols;
		std::list<Scope> m_childScopes;

	public:
		Scope(utf::String name = "", FunctionSymbol* function = nullptr, Scope* parent = nullptr) noexcept;

		Scope& addScope(utf::String name, FunctionSymbol* function);

		std::vector<utils::NoNull<Symbol>>& symbols() noexcept;
		ScopeId			getId()		const noexcept;
		Scope*			getParent() const noexcept;
		FunctionSymbol* getFunction() const noexcept;
		utf::StringView getName()	const noexcept;
		const std::list<Scope>& getChildScopes() const noexcept;

		bool isGlobal() const noexcept;

	private:
		static ScopeId generateUniqueId() noexcept;
	};
}