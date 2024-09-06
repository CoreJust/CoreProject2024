// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirFunctionType.hpp by CoreJust
*	Created on 18.08.2024
*	Contains FunctionType class that is derived from Type and represents a function type in CIR.
*/

#pragma once
#include <vector>
#include "CirType.hpp"

namespace llvm {
	class FunctionType;
}

namespace cir {
	class FunctionType final : public Type {
		template<class T> friend class utils::PooledAllocator;

	protected:
		std::vector<utils::NoNull<Type>> m_argumentTypes;
		utils::NoNull<Type> m_returnType;

	protected:
		FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept;

	public:
		~FunctionType() override;

		static utils::NoNull<FunctionType> make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes);

		llvm::FunctionType* makeRawLLVMFunctionType(llvm::LLVMContext& context) const;
		llvm::Type* makeLLVMType(llvm::LLVMContext& context) const override;
		bool equals(utils::NoNull<Type> other) const noexcept override;

		uint32_t getTypeSize() const noexcept override;
		const std::vector<utils::NoNull<Type>>& getArgumentTypes() const noexcept;
		utils::NoNull<Type> getReturnType() const noexcept;

		utf::String toString() const override;
	};
}