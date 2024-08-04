// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirAllocator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ChirAllocator class that incapsulates a pool allocator for AST nodes allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "ChirNode.hpp"

namespace chir {
	class ChirAllocator final : public utils::PooledAllocator<ast::Node> {
	public:
		// Creates a new CHIR node using the current memory resource.
		template<class T, class... Args> requires std::is_base_of_v<Node, T>
		static utils::NoNull<T> make(utils::TextPosition position, Args&&... args) {
			T* result = reinterpret_cast<T*>(s_allocator.allocate_bytes(sizeof(T), alignof(T)));
			::new(result) T(std::forward<Args>(args)...);

			result->setTextPosition(position);

			return result;
		}
	};
}