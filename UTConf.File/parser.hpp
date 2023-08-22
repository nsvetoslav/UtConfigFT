#pragma once

#include "token_queue.hpp"
#include <map>
#include <variant>
#include <sstream>
#include "wexception.hpp"

namespace utconf {

class parser {
public:
	explicit parser(const token_queue& tokens_ref) noexcept : _tokens(tokens_ref) { }
	parser() = delete;
	parser(const parser& copy) = delete;
	parser(parser& copy) = delete;
	virtual ~parser() = default;

public:
	void execute() {

		std::wstring expecting = L"section";

		validate_expection(expecting, _tokens.front_tok().to_string());
		std::wstring current_param;
		std::wstring current_section;

		while(!_tokens.is_empty()) {
			std::shared_ptr<token> current_token = _tokens.next_token();
			auto current_token_str = current_token->to_string();

			if (current_token_str == L"section") {
				std::shared_ptr<token> next_token = _tokens.next_token();
				
				validate_section_name(next_token.get());
				current_section = next_token.get()->to_string();
				
				next_token = _tokens.next_token();

				validate_doublep(next_token.get());
				expecting = L"param";
				continue;
			}
			
			if (current_token_str == L"param") {
				std::shared_ptr<token> next_token = _tokens.next_token();
				validate_param_name(next_token.get());
				current_param = next_token.get()->to_string();
				
				next_token = _tokens.next_token();
				validate_doublep(next_token.get());
				
				next_token = _tokens.next_token();
				auto param_value_str = next_token.get()->to_string();
				validate_param_value(next_token.get());
				
				next_token = _tokens.next_token();
				validate_end_symbol(next_token.get());

				std::map<std::wstring, std::wstring> sectionParamMap;
				sectionParamMap.insert({ current_section, current_param });

				__ptov_map[sectionParamMap] = param_value_str;
			}
		}
	}

	std::variant<int, double, std::wstring> get_at(std::wstring section_key, std::wstring param) const {
		std::variant<int, double, std::wstring> value;
		
		std::map<std::wstring, std::wstring> sectionParamMap;
		sectionParamMap[section_key] = param;

		auto outerIt = __ptov_map.find(sectionParamMap);
		if (outerIt != __ptov_map.end()) {
			value = outerIt->second;
		}
		else {
			std::wstringstream wsstr;
			wsstr << "Non-existing pair of section "
				<< '"' 
				<< section_key.c_str()
				<< '"' << 
				" and parameter " 
				<< '"' 
				<< param.c_str()
				<< '"';

			throw wexception(wsstr.str());
		}

		return value;
	}

	void validate_expection(const std::wstring& expection, const std::wstring& current_token) {
		if (expection.compare(current_token) != 0) {
			std::wstringstream wsstr;
			wsstr << L"Unexpected token "
				<< L'"'
				<< current_token
				<< L'"'
				<< L". Expected"
				<< "'"
				<< expection
				<< "'";
			throw wexception(wsstr.str());
		}
	}

	void validate_section_name(const token* tok) const {
		validate_not_keyword(tok);
	}

	void validate_doublep(token* tok) {
		if (tok->to_string().compare(L":") != 0) {
			std::wostringstream wsstr;
			wsstr << "Expected double-point keyword after parameter name at line " << tok->line_number();
			throw wexception(wsstr.str());
		}
	}

	void validate_not_keyword(const token* tok) const {
		auto token_str = tok->to_string();
		if (token_str.empty() ||
			token_str.compare(L";") == 0 ||
			token_str.compare(L"section") == 0 ||
			token_str.compare(L"param") == 0) {

			std::wstringstream wsstr;
			wsstr << L"Parameter value "
				<< '"'
				<< token_str
				<< '"'
				<< L" at line "
				<< tok->line_number()
				<< L" can not be a keyword";

			throw wexception(wsstr.str());
		}
	}

	void validate_param_name(const token* tok) const {
		validate_not_keyword(tok);
	}

	void validate_param_value(const token* tok) {
		validate_not_keyword(tok);
	}

	void validate_end_symbol(token* tok) const {
		if (tok->to_string().compare(L";") != 0) {
			std::wstringstream wsstr;
			wsstr << "Expected semi-column at line " << tok->line_number();
			throw wexception(wsstr.str());
		}
	}

private:
	token_queue _tokens;
	std::map<std::map</*section*/std::wstring, /*param*/std::wstring>, std::variant<int, double, std::wstring >/*value*/ > __ptov_map;
};

}