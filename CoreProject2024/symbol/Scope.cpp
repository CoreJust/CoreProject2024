// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Scope.hpp"
#include <atomic>

symbol::Scope::Scope(utf::String name, FunctionSymbol* function, Scope* parent) noexcept
	: m_name(std::move(name)), m_function(function), m_parent(parent), m_id(generateUniqueId()) {

}

symbol::Scope& symbol::Scope::addScope(utf::String name, FunctionSymbol* function) {
	return m_childScopes.emplace_back(std::move(name), function, this);
}

std::vector<utils::NoNull<symbol::Symbol>>& symbol::Scope::symbols() noexcept {
	return m_symbols;
}

symbol::ScopeId symbol::Scope::getId() const noexcept {
	return m_id;
}

symbol::Scope* symbol::Scope::getParent() const noexcept {
	return m_parent;
}

symbol::FunctionSymbol* symbol::Scope::getFunction() const noexcept {
	return m_function;
}

utf::StringView symbol::Scope::getName() const noexcept {
	return m_name;
}

const std::list<symbol::Scope>& symbol::Scope::getChildScopes() const noexcept {
	return m_childScopes;
}

bool symbol::Scope::isGlobal() const noexcept {
	return m_parent == 0;
}

symbol::ScopeId symbol::Scope::generateUniqueId() noexcept {
	static std::atomic<ScopeId> s_idCounter { 1 };

	return s_idCounter++;
}
