#pragma once
#include "../serialize_buffer.h"

#include <intrin.h>
#include <string_view>
#include <iostream>

namespace example {
	inline void serialize_buffer(void) noexcept {
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
}