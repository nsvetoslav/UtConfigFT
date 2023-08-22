#include "utconf_file.hpp"
#include <windows.h>

int main()
{
	try {
		utconf::utconf_file config(L"config.ucf");
		config.open();
		std::wcout<<std::get<std::wstring>(config.at(L"networking", L"ip_address"));
	}
	catch (std::exception ex) {
		std::cerr << ex.what();
	}

	return -1;
}