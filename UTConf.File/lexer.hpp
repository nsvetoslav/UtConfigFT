#pragma once 

#include <string>
#include <fstream>
#include <iostream>
#include "token_queue.hpp"

namespace utconf {

class lexer {
public:
    explicit lexer(std::wifstream& stream) noexcept
        : _stream(stream),
        _line_number(0) {
        tokenize();
    }

	lexer() = delete;
	virtual ~lexer() = default;

public:
	void tokenize() {
		std::wstring ch;

		while (!_stream.eof()) {
			std::wstring line;
			std::getline(_stream, line);
            _line_number+=1;
            tokenize_line(line);
		}
	}

	const token_queue& get_tokens() const {
		return _tokens;
	}	

private:
    void tokenize_line(const std::wstring& line) {
        std::wstring token_name;
        bool inToken = false;

        for (const wchar_t ch : line) {
            if (!inToken && (ch == L' ' || ch == L'\t')) {
                continue;
            }

            if (ch != L' ' && ch != L':' && ch != L';' && ch != L'\t') {
                token_name += ch;
                inToken = true;
            }
            else {
                if (!token_name.empty()) {
                    _tokens.push_back(token(token_name, _line_number));
                    token_name.clear();
                    inToken = false;
                }
                if (ch == L':' || ch == L';') {
                    std::wstring tok;
                    tok += ch;
                    _tokens.push_back(token(tok, _line_number));
                }
            }
        }

        if (!token_name.empty()) {
            _tokens.push_back(token(token_name, _line_number));
        }
    }

private:
	std::wifstream& _stream;
    token_queue _tokens;
    int64_t _line_number;
};

};