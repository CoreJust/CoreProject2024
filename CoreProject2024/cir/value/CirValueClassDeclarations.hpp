// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirValueClassDeclarations.hpp by CoreJust
*	Created on 05.08.2024
*	Contains the declarations of incomplete types for all the Value inheritors.
*	Used in other headers so as not to include the actual CIR Value headers and lighten the compilation.
*/

#pragma once

namespace cir {
	class Value;
	class FunctionArgument;
	class Constant;
	class ConstantNumber;
	class BasicBlock;
	class GlobalValue;
	class Function;
	class NativeFunction;
	class CommonFunction;
	class GlobalVariable;
	class Instruction;
	class UnaryInstruction;
	class BinaryInstruction;
	class InvocationInstruction;
	class UnitInvocationInstruction;
	class LocalVariable;
	class Terminator;
	class GotoInstruction;
	class BranchInstruction;
	class RetInstruction;
}
