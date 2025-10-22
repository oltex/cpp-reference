#pragma once
#include "../string.h"
#include <string>
#include <iostream>

namespace example {
	inline void string(void) noexcept {
		std::string std_string;
		std::wstring std_string2;
		std_string.push_back('a');
		//std_string.at()
		//std_string.insert()
		//std_string.push_back()
		//std_string = "hello";
		std::string_view std_string_view;
		//std_string.insert(std_string_view);
		std_string = std_string_view;
		if (std_string == std_string_view) {
		}


		library::wstring string(L"AAA");
		library::wstring string2(L"BBB");
		detail::string_view<wchar_t> string_view(L"CCC");

		if (string == string2) {}
		if (string == string_view) {}
		if (string == L"DDD") {}
		if (string_view == string) {}
		if (L"DDD" == string) {}
		if (L'E' == string) {};

		auto a = string + string2;
		auto b = string + string_view;
		auto c = string + L"DDD";
		auto d = string + L'E';
		auto e = string_view + string;
		auto f = L"DDD" + string;
		auto g = L'E' + string;

		string += string2;
		string.insert(string.end(), string_view);
		string.insert(string.end(), L"DDD");
		string.insert(string.end(), L'E');

		string.reserve(31);
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
		//library::wstring string2(string);// = L"¹Ý°©°í";
		////string2 = L"ggggg ggggg ggggg ggggg";
		//string2 = std::move(string);

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
	}
}