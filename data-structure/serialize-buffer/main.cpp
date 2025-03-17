#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "serialize_buffer.h"
#include <intrin.h>
#include <string_view>

#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	data_structure::serialize_buffer serialize_buffer;
	{
		serialize_buffer << 0xaaaaaaaa << true << 'c';
		//if (!serialize_buffer)
		//	__debugbreak();
	}
	{
		unsigned int a;
		bool b;
		char c;
		std::string str;

		serialize_buffer >> a >> b >> c;
		//if(!serialize_buffer)
		//	__debugbreak();
	}
}