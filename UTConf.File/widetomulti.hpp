#pragma once

#include <string>
#include <windows.h>

namespace utconf {

class widetomuli {
public:
	static inline const std::string convert(const std::wstring wstr) {
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string str(bufferSize, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
		return str;
	}
};

};
