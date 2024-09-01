// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "InternalAssert.hpp"
#include <cassert>
#include <iostream>
#include "ErrorPrinter.hpp"

void error::internalAssert(bool condition, utf::StringView message) {
	if (!condition) {
		std::cout << "Internal assertion failed: " << message << std::endl;
	}

	assert(condition);
}
