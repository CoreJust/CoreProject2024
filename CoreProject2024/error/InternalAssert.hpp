// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	InternalAssert.hpp by CoreJust
*	Created on 15.08.2024
*	Contains the internalAssert function that causes ErrorPrinter's INTERNAL_ERROR
*	on assertion failure and can be used in Release mode.
*/

#pragma once
#include <source_location>
#include "utf/String.hpp"

namespace error {
	// If the condition is false, causes fatal internal error with the given message for description.
	void internalAssert(bool condition, utf::StringView message = "", std::source_location location = std::source_location::current());
}