// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirType.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Type class that represents a type in CIR.
*	Unlike types in previous phases, this one is more low-level and close to the LLVM types.
*/

#pragma once
#include "utf/String.hpp"
#include "CirTypeKind.hpp"

namespace llvm {
	class Type;
	class LLVMContext;
}

namespace cir {
	/*
	*	The cir::Type is used in Core IR.
	*
	*	Unlike higher-level type system of symbol::Type, this one is more low-level.
	*/
	class Type {
	private:
		TypeKind m_typeKind;

	public:
		Type(TypeKind typeKind) noexcept;

		llvm::Type* makeLLVMType(llvm::LLVMContext& context) const;

		bool operator==(const Type& other) const noexcept;

		TypeKind getTypeKind() const noexcept;

		// Returns the size of the type in bytes.
		uint32_t getTypeSize() const noexcept;

		// Returns the string representation of the type as if in the code.
		utf::String toString() const;
	};
}