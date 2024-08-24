// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirNativeFunction.hpp"

cir::NativeFunction::NativeFunction(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, utils::NoNull<Module> parentModule) noexcept
	: Function(std::move(name), returnType, std::move(arguments), ValueKind::NATIVE_FUNCTION, parentModule) {
}
