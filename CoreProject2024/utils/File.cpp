// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "File.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "utf/Utf.hpp"
#include "error/ErrorPrinter.hpp"

std::optional<utf::String> utils::readFile(const utf::String& path) {
	auto result = readFileUnchecked(path);
	if (!result.has_value()) {
		return result;
	}

	if (!utf::isValidUtf(*result)) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INVALID_UTF_IN_FILE,
			.name = "Compiler error: Invalid Utf-8 encoding of file",
			.description = "Encountered an invalid Utf-8 code when reading file " + path,
		});

		return std::nullopt;
	}

	return result;
}

std::optional<utf::String> utils::readFileUnchecked(const utf::String& path) {
	/*
	*   It is the quickest implementation among all the tested.
	*   Here, we get the file size and allocate a String with the size of the file.
	*   Then, the C functions to work with files are used (fopen and fread), which allow to have close-to-zero overhead.
	*/

	auto fileSize = std::filesystem::file_size(path);
	utf::String result(fileSize, 0);

	std::FILE* file = std::fopen(path.c_str(), "rb");   
	if (file == nullptr) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::CANNOT_OPEN_FILE_ERROR,
			.name = "Compiler error: Failed to open file",
			.description = "Encountered an error when trying to open file " + path,
		});

		return std::nullopt;
	}

	if (fileSize != std::fread(reinterpret_cast<void*>(&result[0]), 1, fileSize, file)) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::CANNOT_OPEN_FILE_ERROR,
			.name = "Compiler error: Failed to read file",
			.description = "Encountered an error when trying to read file " + path,
		});

		std::fclose(file);
		return std::nullopt;
	}

	std::fclose(file);
	return result;
}
