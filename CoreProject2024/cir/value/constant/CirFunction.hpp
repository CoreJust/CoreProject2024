// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirFunction.hpp by CoreJust
*	Created on 06.08.2024
*	Contains Function class that is derived from GlobalValue and represents a function in CIR.
*	It is base for 2 kinds of functions: CommonFunction and NativeFunction.
*/

#pragma once
#include <vector>
#include "CirGlobalValue.hpp"
#include "CirBasicBlock.hpp"
#include "../CirFunctionArgument.hpp"

namespace cir {
	class Function : public GlobalValue {
	protected:
		utils::NoNull<Type> m_returnType;
		std::vector<utils::NoNull<FunctionArgument>> m_arguments;

	public:
		Function(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, ValueKind kind, utils::NoNull<Module> parentModule) noexcept;

		utils::NoNull<Type> getReturnType() const noexcept;
		std::vector<utils::NoNull<FunctionArgument>>& getArguments() noexcept;
	};
}