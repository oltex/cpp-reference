#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "serialize_buffer.h"
#include <intrin.h>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	data_structure::serialize_buffer serialize_buffer;

	{
		std::string str = "hello";
		serialize_buffer << 0xaaaaaaaa << true << 'c';
		serialize_buffer.push<unsigned char>(str);
		//if (!serialize_buffer)
		//	__debugbreak();
	}
	{
		unsigned int a;
		bool b;
		char c;
		std::string str;

		serialize_buffer >> a >> b >> c;
		serialize_buffer.pop<unsigned char>(str);
		//if(!serialize_buffer)
		//	__debugbreak();
	}
}