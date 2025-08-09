#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "string.h"
#include <string>
#include <iostream>
int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::wstring std_string;
	std::wstring std_string2;
	std_string.push_back('a');
	//std_string.at()
	//std_string.insert()
	//std_string.push_back()
	//std_string = "hello";

	library::wstring string(L"AAA");
	//string.reserve(31);
	string.push_back('1');
	string.push_back(L'2');
	string.push_back(L'3');
	string.push_back(L'4');
	string.push_back(L'5');
	string.push_back(L'6');
	//string = L"hello";
	//string += L"guy";
	//auto iter = string.begin();
	//string.insert(iter, L"hello");
	library::wstring string2(string);// = L"¹Ý°©°í";
	//string2 = L"ggggg ggggg ggggg ggggg";
	string2 = std::move(string);

	//string.push_back('1');
	//string.push_back(L'2');
	//string.push_back(L'3');
	//string.push_back(L'4');
	//string.push_back(L'5');
	//string.push_back(L'6');
	//string = L"hello";
	//string += L"guy";

	//if (string2 == string) {
	//	int a = 10;
	//}
	//string2 = L'A';
	//if (L'B' != string2) {
	//	int a = 10;
	//}


	//detail::string<wchar_t> string3;

	//string2.swap(string);
	return 0;
}