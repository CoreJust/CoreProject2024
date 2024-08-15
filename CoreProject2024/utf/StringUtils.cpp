// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "StringUtils.hpp"
#include <format>
#include "UtfIterator.hpp"

utf::String utf::replaceControlCharacters(utf::StringView origin) {
	utf::String result;
	result.reserve(origin.size());

	for (Char ch : charsOf(origin)) {
		switch (ch) {
			case encodeUtf('\n'): result += "\\n"; break;
			case encodeUtf('\v'): result += "\\v"; break;
			case encodeUtf('\f'): result += "\\f"; break;
			case encodeUtf('\r'): result += "\\r"; break;
			case encodeUtf('\t'): result += "\\t"; break;
			case encodeUtf('\a'): result += "\\a"; break;
			case encodeUtf('\b'): result += "\\b"; break;
			case encodeUtf('\\'): result += "\\"; break;
		default:
			if (isControlCharacter(ch)) {
				result += std::format("\\U{{{:X}}}", decodeUtf(ch));
			} else {
				result += ch;
			}

			break;
		}
	}

	return result;
}
