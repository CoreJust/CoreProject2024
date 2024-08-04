// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ModuleDeclarations.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ModuleDeclarations class derived from ast::Declaration.
*	It represents the module's (= source file's) declarations list, similar to ScopeStatement.
*/

#pragma once
#include <vector>
#include "../Declaration.hpp"

namespace ast {
	class ModuleDeclarations final : public Declaration {
	private:
		std::vector<utils::NoNull<Declaration>> m_declarations;

	public:
		ModuleDeclarations(std::vector<utils::NoNull<Declaration>> declarations) noexcept;

		const std::vector<utils::NoNull<Declaration>>& getDeclarations() const noexcept;
	};
}
