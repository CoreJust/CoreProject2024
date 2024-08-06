// This is an open source non - commercial project.Dear PVS - Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	PooledAllocator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the PooledAllocator class that is base for other *Allocator classes for pooled allocations.
*/

#pragma once
#include <memory_resource>
#include "NoNull.hpp"

namespace utils {
	// ObjectBase is the base class for all the objects that are allocated with PooledAllocator's derived classes.
	template<class ObjectBase>
	class PooledAllocator {
	protected:
		inline static thread_local std::pmr::unsynchronized_pool_resource s_resource { };
		inline static thread_local std::pmr::polymorphic_allocator<ObjectBase> s_allocator { &s_resource };

	public:
		// Frees all the memory.
		// If the memory was already released, does nothing.
		static void tryRelease() {
			s_resource.release();
		}

		// Creates a new object using the current memory resource.
		template<class T, class... Args> requires std::is_base_of_v<ObjectBase, T>
		static NoNull<T> make(Args&&... args) {
			T* result = reinterpret_cast<T*>(s_allocator.allocate_bytes(sizeof(T), alignof(T)));
			::new(result) T(std::forward<Args>(args)...);

			return result;
		}
	};
}