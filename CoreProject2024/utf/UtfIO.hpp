// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	UtfIO.hpp/.cpp by CoreJust
*	Created on 19.07.2024
*	Contains a function for I/O initialization for Utf-8 as well as operators for Utf-8 I/O.
*/

namespace utf {
	// On Windows, the console cannot use Utf-8 by default, so we set it up.
	// This functions tries to set up Utf-8 console mode and returns error code on failure.
	// Returns 0 on success.
	int initUtfIO();
}