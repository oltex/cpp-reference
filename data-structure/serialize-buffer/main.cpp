#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "serialize_buffer.h"
#include "../pool/pool.h"
#include <intrin.h>
#include <string_view>

#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::serialize_buffer<128> serialize_buffer;
	library::serialize_buffer<0> serialize_buffer2;
	{
		serialize_buffer << 0xaaaaaaaa << true << 'c';
	}
	{
		unsigned int a;
		bool b;
		char c;
		serialize_buffer >> a >> b >> c;
		int d = 10;
	}
}