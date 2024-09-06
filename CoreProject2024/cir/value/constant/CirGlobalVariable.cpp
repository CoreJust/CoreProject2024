// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGlobalVariable.hpp by CoreJust
*	Created on 05.08.2024
*	Contains GlobalVariable class that is derived from GlobalValue and represents a global variable in CIR.
*/

#include "CirGlobalVariable.hpp"
#include <cassert>

cir::GlobalVariable::GlobalVariable(utf::String name, utils::NoNull<Type> type, utils::NoNull<Constant> initialValue, utils::NoNull<Module> parentModule) noexcept
	: GlobalValue(std::move(name), type, ValueKind::GLOBAL_VARIABLE, parentModule), m_initialValue(initialValue) {
	assert(m_type->getTypeKind() == cir::TypeKind::UNIT);
	assert(m_type == m_initialValue->getType());

	Value::addUser(m_initialValue, *this);
}

cir::GlobalVariable::~GlobalVariable() {
	m_initialValue->~Constant();
}

utils::NoNull<cir::Constant>& cir::GlobalVariable::getInitialValue() noexcept {
	return m_initialValue;
}
