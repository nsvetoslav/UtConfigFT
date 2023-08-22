#pragma once

#include <string>
#include "lexer.hpp"
#include "parser.hpp"

namespace utconf {

class utconf_file {
public:
	explicit utconf_file(const std::wstring filename) noexcept
		: _filename(filename),
		_opened(false) { }

	utconf_file(const utconf_file& ref) = default;
	utconf_file(utconf_file& ref) = delete;
	utconf_file() = delete;

	virtual ~utconf_file() noexcept {
		if (_opened)
			_wifstream.close();
	}

public:
	void open(std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot) {
		try {
			if (_opened)
				throw std::runtime_error("File is already opened.");

			_wifstream.open(_filename, _Mode, _Prot);
			_opened = true;
		} catch (std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
	}

	const std::variant<int, double, std::wstring> at(const std::wstring section, const std::wstring param) {
		lexer lexer(get_stream());

		parser syntax_val(lexer.get_tokens());
		syntax_val.execute();

		return syntax_val.get_at(section, param);
	}

private:
	std::wifstream& get_stream() {
		return _wifstream;
	}

private:
	std::wstring _filename;
	std::wifstream _wifstream;
	bool _opened;
};

};