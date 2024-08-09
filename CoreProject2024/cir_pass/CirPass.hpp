// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirPass.hpp by CoreJust
*	Created on 06.08.2024
*	Contains several base classes for traversing Core IR.
*/

#pragma once
#include "utils/NoNull.hpp"

namespace cir {
	class Module;
	class GlobalValue;
	class Function;
	class CommonFunction;
	class BasicBlock;
	class Instruction;
}

namespace cir_pass {
	/*
	*	There are 4 general kinds of Passes:
	*	1) ModulePass is used to traverse CIR Modules.
	*	2) GlobalPass is used to traverse CIR Global Values (functions and global variables).
	*	   There is a more specific FunctionPass specifically for functions.
	*	   And an even more specific one, CommonFunctionPass for common functions.
	*	3) BasicBlockPass is used to traverse Basic Blocks.
	*	4) InstructionPass is used to traverse separate Instructions.
	* 
	*	All the passes have a pass(...) method that returns void.
	*/

	class ModulePass {
	public:
		virtual void pass(utils::NoNull<cir::Module> module) = 0;
	};

	class GlobalValuePass {
	public:
		virtual void pass(utils::NoNull<cir::GlobalValue> value) = 0;
	};

	class FunctionPass {
	public:
		virtual void pass(utils::NoNull<cir::Function> function) = 0;
	};

	class CommonFunctionPass {
	public:
		virtual void pass(utils::NoNull<cir::CommonFunction> function) = 0;
	};

	class BasicBlockPass {
	public:
		virtual void pass(utils::NoNull<cir::BasicBlock> basicBlock) = 0;
	};

	class InstructionPass {
	public:
		virtual void pass(utils::NoNull<cir::Instruction> instruction) = 0;
	};
}