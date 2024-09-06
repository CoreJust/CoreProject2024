// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirModule.hpp by CoreJust
*	Created on 06.08.2024
*	Contains Module class that contains a CIR Module
*	with a list of functions and global definitions.
*/

#pragma once
#include "value/constant/CirGlobalValue.hpp"

namespace cir {
	class GlobalVariable;
	class NativeFunction;
	class CommonFunction;
	class FunctionArgument;

	class Module final {
	private:
		utf::String m_name;
		std::vector<utils::NoNull<GlobalValue>> m_globals;
		
		// Special function that initializes the global variables.
		utils::NoNull<cir::CommonFunction> m_globalConstructor;

	public:
		Module(utf::String name) noexcept;
		~Module();

		// Prints the module in a human-friendly manner to the given stream.
		void print(std::ostream& out);

		utils::NoNull<GlobalVariable> addGlobalVariable(utf::String name, utils::NoNull<Type> type, utils::NoNull<Constant> initialValue);
		utils::NoNull<NativeFunction> addNativeFunction(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments);
		utils::NoNull<CommonFunction> addCommonFunction(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments);

		utf::String getName() const noexcept;
		std::vector<utils::NoNull<GlobalValue>>& getGlobals() noexcept;
		utils::NoNull<cir::CommonFunction> getGlobalConstructor() const noexcept;
	};
}