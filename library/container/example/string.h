#pragma once
#include "../string.h"
#include "../hash_table.h"
#include "../../system/guid.h"
#include <string>
#include <iostream>

namespace example {
	inline void string(void) noexcept {
		{
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
		}
		{
			library::wstring string(L"AAA");
			library::wstring string2(L"BBB");
			detail::string_view<wchar_t> string_view(L"CCC");
			library::wstring string3;

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
		}

		{
			library::unorder_set<library::wstring> table;
			table.emplace(L"aaa");
			auto b = table.find(library::wstring_view(L"bbb"));
			int a = 10;
		}
		{
			library::string string("hello");
			library::wstring wstring(L"æ»≥Á«œººø‰");
			library::string string_view(L"hello");
			library::wstring_view wstring_view(L"æ»≥Á«œººø‰");
			char char_word = 'h';
			wchar_t wchar_word = L'æ»';
			char const* char_string = "hello";
			wchar_t const* wchar_string = L"æ»≥Á«œººø‰";

			auto a = string + wstring;
			auto b = string + wstring_view;
			auto c = string + wchar_word;
			auto d = string + wchar_string;

			auto e = wstring + string;
			auto f = wstring + string_view;
			auto g = wstring + char_word;
			auto h = wstring + char_string;

			int k = 10;

			string = "gggg";
			library::guid guid = library::create_guid();
			//string.insert(string.end() - 2, guid);
			string = guid;
			//wstring = "gggg";
			//wstring.insert(wstring.end() - 2, guid);
			wstring = guid;

			library::guid guid2 = string;
			if (guid2 == guid)
				int a = 10;
			library::guid guid3 = wstring;
			if (guid3 == guid)
				int a = 10;

			library::guid guid4 = guid;
			if (guid4 == guid)
				int a = 10;

		}


		//string = L"hello";
		//string += L"guy";
		//auto iter = string.begin();
		//string.insert(iter, L"hello");
		//library::wstring string2(string);// = L"π›∞©∞Ì";
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