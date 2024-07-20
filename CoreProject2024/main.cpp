/*
*	main.cpp by CoreJust
*	Created on 18.07.2024
*	The main source file of the program that contains the entry point (function main()).
*/

#include <iostream>
#include "utils/UtfIO.hpp"

// The entry point of the CoreProject2024.
int main() {
	// Test utf-8
	if (auto code = initUtfIO(); code != 0) {
		exit(code);
	}

	String str = u8"\u20ac\u20ac";
	std::cout << str << std::endl;

	return 0;
}