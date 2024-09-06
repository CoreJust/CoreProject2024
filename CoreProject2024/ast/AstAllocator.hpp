// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstAllocator.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the AstAllocator class that incapsulates a pool allocator for AST nodes allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "utils/TextPosition.hpp"
#include "Node.hpp"

namespace ast {
	class AstAllocator final : public utils::PooledAllocator<ast::Node> {
	public:
		// Creates a new AST node using the current memory resource.
		template<class T, class... Args> requires std::is_base_of_v<Node, T>
		static utils::NoNull<T> make(utils::TextPosition position, Args&&... args) {
			utils::NoNull<T> result = utils::PooledAllocator<ast::Node>::make<T, Args...>(std::forward<Args>(args)...);

			result->setTextPosition(position);
			return result;
		}
	};
}