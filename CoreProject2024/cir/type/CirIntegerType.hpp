// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirIntegerType.hpp by CoreJust
*	Created on 18.08.2024
*	Contains IntegerType class that is derived from Type and represents an integer type in CIR.
*/

#pragma once
#include "CirType.hpp"

namespace cir {
	class IntegerType final : public Type {
		template<class T> friend class utils::PooledAllocator;

	protected:
		uint32_t m_bitSize;
		bool m_isSigned;

	protected:
		IntegerType(uint32_t bitSize, bool isSigned) noexcept;

	public:
		static utils::NoNull<IntegerType> make(uint32_t bitSize, bool isSigned);

		llvm::Type* makeLLVMType(llvm::LLVMContext& context) const override;
		bool equals(utils::NoNull<Type> other) const noexcept override;

		uint32_t getTypeSize() const noexcept override;
		uint32_t getBitSize() const noexcept;
		bool isSigned() const noexcept;

		utf::String toString() const override;
	};
}