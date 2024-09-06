// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	InvocationOperator.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the InvocationOperator class that implements the AST node for an invocation operator in the Core source code.
*/

#pragma once
#include <vector>
#include "../Expression.hpp"

namespace ast {
	class InvocationOperator final : public Expression {
	private:
		utils::NoNull<Expression> m_callee;
		std::vector<utils::NoNull<Expression>> m_arguments;

	public:
		InvocationOperator(utils::NoNull<Expression> callee, std::vector<utils::NoNull<Expression>> arguments) noexcept;
		~InvocationOperator() override;

		utils::NoNull<Expression>& getCallee() noexcept;
		std::vector<utils::NoNull<Expression>>& getArguments() noexcept;
	};
}