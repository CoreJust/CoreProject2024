// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGlobalVariable.hpp by CoreJust
*	Created on 05.08.2024
*	Contains GlobalVariable class that is derived from GlobalValue and represents a global variable in CIR.
*/

#pragma once
#include "CirGlobalValue.hpp"

namespace cir {
	class GlobalVariable final : public GlobalValue {
	private:
		utils::NoNull<Constant> m_initialValue;

	public:
		GlobalVariable(utf::String name, utils::NoNull<Type> type, utils::NoNull<Constant> initialValue, utils::NoNull<Module> parentModule) noexcept;

		utils::NoNull<Constant>& getInitialValue() noexcept;
	};
}