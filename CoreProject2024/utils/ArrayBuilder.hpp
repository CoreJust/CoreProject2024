// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ArrayBuilder.hpp by CoreJust
*	Created on 02.09.2024
*	Contains the ArrayBuilder class that allows to create an array and then use it in llvm::ArrayRefs.
*/

#pragma once
#include <llvm/ADT/ArrayRef.h>

namespace utils {
#if 0
	// Allows to destroy an object on deallocation.
	using Destructor = void(*)(std::byte*);

	// Helper function that calls the object's destructor (since a pointer to a destructor cannot be obtained directly).
	template<class T>
	void destructorImpl(std::byte* ptr) noexcept {
		reinterpret_cast<T*>(ptr)->~T();
	}

	// Registers the destructor and returns an index that must then be used to address the destructor.
	// Just returns the index if the destructor was already registered.
	uint16_t getDestructorIndex(Destructor destructor);

	/*
	* Contains info on an allocated object.
	* For memory usage reduction it compresses the info:
	*	Instead of full destructor pointers, only a single entry of destructor pointer is stored,
	* while in the node we store the index of the destructor.
	*	Also, the data pointer is divided into 2 parts: the lower part (least 4 bytes) are stored as is,
	* whereas the higher 4 bytes are stored in a separate vector and only their index is stored there.
	* It makes use of the fact that a lot of pointers differ only in the last bytes, while their beginnings are mostly the same.
	*/
	struct AllocationNode final {
		uint32_t ptrLo;
		uint16_t ptrHiIndex;
		uint16_t destructorIndex; // We suppose there would be no more than 2**16 types with different destructors.
	};

	// Helper class that incapsulated the pooled allocator's logic.
	class PooledAllocatorImpl final {
	private:
		std::vector<AllocationNode> m_nodes;
		std::pmr::unsynchronized_pool_resource m_resource;

	public:
		void release();

		std::byte* allocate(size_t size, size_t align, uint16_t destructorIndex);
	};
#endif

	void releaseArrayAllocator();

	// Uses polymorphic allocator to allocate (if old data is nullptr) or reallocate memory.
	// Size is given in bytes, no initialization is done.
	std::byte* reallocateData(std::byte* oldData, size_t oldSize, size_t newSize, size_t align);

	// Stores the Array destructor after the building is done.
	void finalize(void(*destructor)(std::byte*), std::byte* ptr, size_t objectSize, size_t objectCount);

	template<class T>
	class ArrayBuilder final {
	private:
		T* m_data;
		size_t m_size;

	private:
		static void destructorImpl(std::byte* ptr) noexcept {
			reinterpret_cast<T*>(ptr)->~T();
		}

		T* realloc() {
			m_data = reinterpret_cast<T*>(reallocateData(reinterpret_cast<std::byte*>(m_data), m_size * sizeof(T), (m_size + 1) * sizeof(T), alignof(T)));
			return m_data + m_size++;
		}

	public:
		// Creates an ArrayBuilder with the given number of allocated (but not initialized) elements.
		ArrayBuilder(size_t initialSize = 0) : m_size(initialSize), m_data(nullptr) {
			if (initialSize != 0) {
				m_data = reinterpret_cast<T*>(reallocateData(nullptr, 0, initialSize * sizeof(T), alignof(T)));
			}
		}

		T& operator[](size_t index) noexcept {
			return m_data[index];
		}

		template<class... Args>
		T& emplace_back(Args... args) {
			T* ptr = realloc();
			::new(ptr) T(std::forward<Args...>(args)...);
			return *ptr;
		}

		llvm::ArrayRef<T> finalize() const noexcept {
			utils::finalize(destructorImpl, reinterpret_cast<std::byte*>(m_data), sizeof(T), m_size);
			return llvm::ArrayRef<T>(m_data, m_size);
		}
	};
}
