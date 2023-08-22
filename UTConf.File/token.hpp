#pragma once

#include <string>

namespace utconf{

class token {
public:
	explicit token(const token& token) noexcept
		: _token(token.to_string()),
		_line_number(token.line_number()) {}

	explicit token(token& token) noexcept
		: _token(token.to_string()),
		_line_number(token.line_number()) {}
	
	explicit token(const token&& other) noexcept
		: _token(other.to_string()),
		_line_number(other.line_number()) {}

	explicit token(std::wstring& token_name, const int64_t& line_number) noexcept
		: _token(token_name),
		_line_number(line_number) {}

	explicit token(const wchar_t* charr, const int64_t& line_number) noexcept
		: _token(std::wstring(charr)),
		_line_number(line_number) {}

	token() = default;
	virtual ~token() = default;

public:
	const std::wstring to_string() const noexcept {
		return _token;
	}

	int64_t line_number() const {
		return _line_number;
	}

private:
	std::wstring _token;
	int64_t _line_number;
};

};
