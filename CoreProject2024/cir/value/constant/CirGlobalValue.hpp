// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGlobalValue.hpp by CoreJust
*	Created on 05.08.2024
*	Contains GlobalValue class that is derived from Constant and is base for all global entities in CIR.
*/

#pragma once
#include "CirConstant.hpp"

namespace cir {
	class Module;

	class GlobalValue : public Constant {
	private:
		utils::NoNull<Module> m_parentModule;

	public:
		GlobalValue(utf::String name, Type type, ValueKind kind, utils::NoNull<Module> parentModule) noexcept;

		Module& getParentModule() noexcept;
	};
}