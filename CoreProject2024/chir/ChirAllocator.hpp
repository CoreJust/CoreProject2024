// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirAllocator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ChirAllocator class that incapsulates a pool allocator for CHIR nodes allocations.
*/

#pragma once
#include "utils/PooledAllocator.hpp"
#include "ChirNode.hpp"

namespace chir {
	class ChirAllocator final : public utils::PooledAllocator<chir::Node> {
	public:
		// Creates a new CHIR node using the current memory resource.
		template<class T, class... Args> requires std::is_base_of_v<Node, T>
		static utils::NoNull<T> make(utils::TextPosition position, Args&&... args) {
			utils::NoNull<T> result = utils::PooledAllocator<chir::Node>::make<T, Args...>(std::forward<Args>(args)...);

			result->setTextPosition(position);
			return result;
		}
	};
}