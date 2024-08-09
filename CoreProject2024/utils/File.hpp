// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	File.hpp by CoreJust
*	Created on 26.07.2024
*	Contains some auxiliary utilities for working with files and file system.
*/

#pragma once
#include <optional>
#include "utf/String.hpp"

namespace utils {
	// Reads the contents of the file on the given path.
	// If the contents of the file is not a valid Utf-8 string, or the file cannot be read, returns nullopt.
	std::optional<utf::String> readFile(const utf::String& path);

	// Reads the contents of the file on the given path.
	// Does not check for correct Utf-8 contents.
	// Can return nullopt in case the file cannot be read.
	std::optional<utf::String> readFileUnchecked(const utf::String& path);
}