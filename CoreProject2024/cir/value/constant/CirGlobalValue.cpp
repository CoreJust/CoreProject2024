// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirGlobalValue.hpp"
#include <cassert>
#include "cir/CirModule.hpp"

cir::GlobalValue::GlobalValue(utf::String name, utils::NoNull<Type> type, ValueKind kind, utils::NoNull<Module> parentModule) noexcept
	: Constant(std::move(name), type, kind), m_parentModule(parentModule) {
	assert(cir::isGlobalValue(kind));
}

cir::Module& cir::GlobalValue::getParentModule() noexcept {
	return *m_parentModule;
}
