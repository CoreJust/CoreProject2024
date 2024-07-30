// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstAllocator.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the AstAllocator class that incapsulates an arena for AST nodes allocations.
*/

#pragma once
#include <memory_resource>
#include "Node.hpp"

namespace ast {
	class AstAllocator final {
	private:
		static thread_local std::pmr::unsynchronized_pool_resource s_resource;
		static thread_local std::pmr::polymorphic_allocator<> s_allocator;

	public:
		// Frees all the memory.
		static void clear();

		// Creates a new AST Node using the current memory resource.
		template<class T, class... Args> requires std::is_base_of_v<Node, T>
		static T* make(Args&&... args) {
			T* result = reinterpret_cast<T*>(s_allocator.allocate_bytes(sizeof(T), alignof(T)));
			::new(result) T(std::forward<Args>(args)...);

			return result;
		}
	};
}