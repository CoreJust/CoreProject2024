// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "UtfIO.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif // _WIN32

int utf::initUtfIO() {
#ifdef _WIN32
	fflush(stdout);

	// First of all, we need to change the codepage in the console into Utf-8
	if (!IsValidCodePage(CP_UTF8)) return GetLastError();
	if (!SetConsoleCP(CP_UTF8)) return GetLastError();
	if (!SetConsoleOutputCP(CP_UTF8)) return GetLastError();

	// Then, we add buffering (because otherwise utf-8 characters may be passed byte-by-byte, which will break the output).
	// setvbuf(stdout, nullptr, _IOFBF, 1024);
#endif // _WIN32

	// Additionally we turn sync with C stdio off so as to accelerate I/O operations.
	std::ios_base::sync_with_stdio(false);
	return 0;
}
