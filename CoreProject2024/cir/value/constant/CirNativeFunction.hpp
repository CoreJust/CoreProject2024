// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirNativeFunction.hpp by CoreJust
*	Created on 06.08.2024
*	Contains NativeFunction class that is derived from Function
*	and represents an external (native) function in CIR.
*/

#pragma once
#include "CirFunction.hpp"

namespace cir {
	class NativeFunction final : public Function {
	public:
		NativeFunction(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, utils::NoNull<Module> parentModule) noexcept;
	};
}