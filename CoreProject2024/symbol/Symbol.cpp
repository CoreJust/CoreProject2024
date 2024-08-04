// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Symbol.hpp"
#include <atomic>

symbol::Symbol::Symbol(SymbolPath path, utf::String name, SymbolKind kind) noexcept
    : m_path(std::move(path)), m_name(std::move(name)), m_kind(kind), m_id(generateUniqueId()) { }

const symbol::SymbolPath& symbol::Symbol::getSymbolPath() const noexcept {
    return m_path;
}

const utf::String& symbol::Symbol::getName() const noexcept {
    return m_name;
}

symbol::SymbolId symbol::Symbol::getId() const noexcept {
    return m_id;
}

symbol::SymbolKind symbol::Symbol::getKind() const noexcept {
    return m_kind;
}

symbol::SymbolId symbol::Symbol::generateUniqueId() noexcept {
    static std::atomic<SymbolId> s_idCounter { 1 };

    return s_idCounter++;
}
