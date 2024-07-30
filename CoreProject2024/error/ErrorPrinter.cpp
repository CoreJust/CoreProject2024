// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ErrorPrinter.hpp"
#include <format>
#include <iostream>

void error::ErrorPrinter::error(const ErrorStruct& err) {
	printMessage('E', err);
}

void error::ErrorPrinter::warning(const ErrorStruct& err) {
	printMessage('W', err);
}

void error::ErrorPrinter::setSource(utf::StringView source) {
	s_currentSource = source;
}

void error::ErrorPrinter::printMessage(char messageType, const ErrorStruct& err) {
	switch (errorDetailLevel) {
		case ErrorDetailLevel::ERROR_NAME_AND_CODE_ONLY:
			std::cout << std::format("[{}{:0>4}] {} at {}", messageType, err.code, err.name, err.selectionStart.toString()) << std::endl;
			break;
		case ErrorDetailLevel::WITH_CODE:
			std::cout << std::format(
				"[{}{:0>4}] {} at {}\n{}\n", 
				messageType, 
				err.code, 
				err.name,
				err.selectionStart.toString(),
				err.selectionStart.getTextSelection(s_currentSource, err.selectionLength)
			) << std::endl;
			break;
		case ErrorDetailLevel::WITH_DESCRIPTION:
			std::cout << std::format(
				"[{}{:0>4}] {} at {}\n{}\nDescription: {}\n",
				messageType,
				err.code,
				err.name,
				err.selectionStart.toString(),
				err.selectionStart.getTextSelection(s_currentSource, err.selectionLength),
				err.description
			) << std::endl;
			break;
		case ErrorDetailLevel::WITH_EXPLANATION:
			std::cout << std::format(
				"[{}{:0>4}] {} at {}\n{}\nDescription: {}\nExplanation: {}\n",
				messageType,
				err.code,
				err.name,
				err.selectionStart.toString(),
				err.selectionStart.getTextSelection(s_currentSource, err.selectionLength),
				err.description,
				err.explanation
			) << std::endl;
			break;
	default: break;
	}
}
