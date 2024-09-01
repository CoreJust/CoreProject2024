// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirType.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Type class that represents a type in CIR.
*	Unlike types in previous phases, this one is more low-level and close to the LLVM types.
*/

#pragma once
#include "utils/NoNull.hpp"
#include "utils/PooledAllocator.hpp"
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
		template<class T> friend class utils::PooledAllocator;

	protected:
		TypeKind m_typeKind;

	protected:
		Type(TypeKind typeKind) noexcept;

	public:
		static utils::NoNull<Type> make(TypeKind typeKind);

		virtual llvm::Type* makeLLVMType(llvm::LLVMContext& context) const;

		virtual bool equals(utils::NoNull<Type> other) const noexcept;

		TypeKind getTypeKind() const noexcept;

		// Returns the size of the type in bytes.
		virtual uint32_t getTypeSize() const noexcept;

		// Returns the string representation of the type as if in the code.
		virtual utf::String toString() const;
	};
}