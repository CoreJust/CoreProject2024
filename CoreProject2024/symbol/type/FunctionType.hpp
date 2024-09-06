// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	FunctionType.hpp by CoreJust
*	Created on 18.08.2024
*	Contains the FunctionType class that is derived from Type and is used in symbols for function types.
*/

#pragma once
#include <vector>
#include "Type.hpp"

namespace symbol {
	class FunctionType final : public Type {
		template<class T> friend class utils::PooledAllocator;

	private:
		std::vector<utils::NoNull<Type>> m_argumentTypes;
		utils::NoNull<Type> m_returnType;

	protected:
		FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept;

	public:
		static utils::NoNull<FunctionType> make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes);

	public:
		~FunctionType() override;

		utils::NoNull<cir::Type> makeCirType() const override;
		bool equals(const Type& other) const noexcept override;
		utf::String toString() const noexcept override;
		utf::String toMangleString() const noexcept override;

		const std::vector<utils::NoNull<Type>>& getArgumentTypes() const noexcept;
		utils::NoNull<Type> getReturnType() const noexcept;
	};
}