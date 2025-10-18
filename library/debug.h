#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <intrin.h>
//#ifdef _DEBUG
//	#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#endif

namespace library {
	inline auto crt_set_debug_flag(void) noexcept {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
	inline auto crt_dump_memory_leak(void) noexcept {
		_CrtDumpMemoryLeaks();
	}
	inline auto crt_set_break_allocate(long new_value) noexcept {
		_CrtSetBreakAlloc(new_value);
	}

	inline void debug_break(void) noexcept {
		::__debugbreak();
	}
}