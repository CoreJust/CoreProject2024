// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstFunctionType.hpp by CoreJust
*	Created on 18.08.2024
*	Contains the FunctionType class that is derived from Type and is used in AST for function types.
*/

#pragma once
#include <vector>
#include "AstType.hpp"

namespace ast {
	class FunctionType final : public Type {
		template<class T> friend class utils::PooledAllocator;

	private:
		std::vector<utils::NoNull<Type>> m_argumentTypes;
		utils::NoNull<Type> m_returnType;

	protected:
		FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept;
		~FunctionType() override;

	public:
		static utils::NoNull<FunctionType> make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes);

	public:
		utils::NoNull<symbol::Type> makeSymbolType() const override;
		utf::String toString() const noexcept override;

		const std::vector<utils::NoNull<Type>>& getArgumentTypes() const noexcept;
		utils::NoNull<Type> getReturnType() const noexcept;
	};
}