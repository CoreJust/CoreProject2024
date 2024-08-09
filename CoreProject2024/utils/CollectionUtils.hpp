// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	VectorUtils.hpp by CoreJust
*	Created on 02.08.2024
*	Contains some auxiliary functions for vectors.
*/

#pragma once
#include "Concept.hpp"
#include "utf/String.hpp"
#include "utf/StringUtils.hpp"

namespace utils {
	// LinearContainer is any object that have a size and that can be iterated over.
	template<class T>
	concept LinearContainer = requires(std::remove_cvref_t<T> container) {
		{ container.size() } -> std::same_as<size_t>;
		container.begin();
		container.end();
	};

	// Concept for types with reserve method.
	template<class T>
	concept Reservable = requires(T t) {
		t.reserve(size_t(0));
	};

	// Concept for types with emplace_back() method.
	template<class T, class Elem = ElementTy<T>>
	concept BackEmplaceable = LinearContainer<T> && requires(std::remove_cvref_t<T> t, Elem elem) {
		t.emplace_back(elem);
	};

	// Results in the type of container element.
	template<LinearContainer T>
	using ElementTy = std::remove_cvref_t<decltype(*std::remove_cvref_t<T>().begin())>;

	// Compares the objects of 2 containers and returns true if all the corresponding elements of the containers are equal.
	bool areEqual(const LinearContainer auto& left, const LinearContainer auto& right)
		requires std::equality_comparable_with<decltype(*left.begin()), decltype(*right.begin())> {
		if (left.size() != right.size()) {
			return false;
		}

		auto leftIt = left.begin();
		auto rightIt = right.begin();
		const auto leftEnd = left.end();
		while (leftIt != leftEnd) {
			if (*leftIt != *rightIt) {
				return false;
			}

			++leftIt;
			++rightIt;
		}

		return true;
	}

	// Compares the objects of 2 containers and returns true if all the corresponding elements of the containers are equal.
	// Can get a custom comparison function.
	bool areEqual(const LinearContainer auto& left, const LinearContainer auto& right, const auto& compare)
		requires Callable<decltype(compare), bool, decltype(*left.begin()), decltype(*right.begin())> {
		if (left.size() != right.size()) {
			return false;
		}

		auto leftIt = left.begin();
		auto rightIt = right.begin();
		const auto leftEnd = left.end();
		while (leftIt != leftEnd) {
			if (!compare(*leftIt, *rightIt)) {
				return false;
			}

			++leftIt;
			++rightIt;
		}

		return true;
	}

	// Helper class.
	template<class T>
	class DefaultTransform final {
	public:
		utf::String operator()(const T& value) const noexcept {
			return utf::toString(value);
		}
	};

	// Joins the given container to String, using the given <transform>.
	// The elements of container are separated with <separator>, <prefix> is 
	// appended to the beginning of the string, <postfix> to the end.
	template<LinearContainer ContainerTy, Callable<utf::String, ElementTy<ContainerTy>> TransformTy = DefaultTransform<ElementTy<ContainerTy>>>
	utf::String joinToString(
		const ContainerTy& container,
		utf::StringView separator = ", ",
		utf::StringView prefix = "",
		utf::StringView postfix = "",
		const TransformTy& transform = TransformTy()
	) {
		utf::String result(prefix);
		
		auto it = container.begin();
		const auto end = container.end();
		if (it != end) {
			result += transform(*it);
			++it;
		}

		while (it != end) {
			result += separator;
			result += transform(*it);
			++it;
		}

		result += postfix;
		return result;
	}

	// Implements the map operation that converts elements of one container into the elements of another one.
	template<LinearContainer ResultTy, LinearContainer SourceTy, Callable<ElementTy<ResultTy>, ElementTy<SourceTy>> TransformTy>
	ResultTy map(const SourceTy& source, const TransformTy& transform) {
		ResultTy result = ResultTy();
		if constexpr (Reservable<ResultTy>) {
			result.reserve(source.size());
		}

		for (const auto& elem : source) {
			if constexpr (BackEmplaceable<ResultTy>) {
				result.emplace_back(transform(elem));
			} else {
				result.emplace(transform(elem));
			}
		}

		return result;
	}
}