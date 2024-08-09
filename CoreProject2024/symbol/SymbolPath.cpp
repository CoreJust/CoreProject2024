// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SymbolPath.hpp"
#include "utils/CollectionUtils.hpp"

utf::String symbol::Path::toString() const noexcept {
    return utils::joinToString(path, ".");
}
