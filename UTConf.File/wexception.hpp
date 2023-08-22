#pragma once

#include "widetomulti.hpp"
#include <iostream>

namespace utconf {

class wexception : public std::exception {
public:
	explicit wexception(const std::wstring exception) noexcept
		: _exception(exception)
		, std::exception(widetomuli::convert(exception).c_str()) { }

	constexpr const wchar_t* const what() noexcept {
		return _exception.c_str();
	}

private:
	const std::wstring _exception;
};

};