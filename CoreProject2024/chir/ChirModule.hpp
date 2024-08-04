// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirModule.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Module class that implement a single core module on Core HIR level.
*/

#pragma once
#include <vector>
#include "ChirDeclaration.hpp"
#include "utils/IndentPrinter.hpp"

namespace chir {
	class Module final {
	private:
		std::vector<utils::NoNull<Declaration>> m_declarations;

	public:
		Module(std::vector<utils::NoNull<Declaration>> declarations) noexcept;

		void print(utils::IndentPrinter& printer) const;
	};
}