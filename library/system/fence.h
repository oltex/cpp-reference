#pragma once
#include <Windows.h>

namespace library {
	inline void load_fence(void) noexcept {
		_mm_lfence();
	}
	inline void store_fence(void) noexcept {
		_mm_sfence();
	}
	inline void memory_fence(void) noexcept {
		_mm_mfence();
	}
	inline void fast_store_fence(void) noexcept {
		__faststorefence();
	}
	inline void flush(void const* address) noexcept {
		_mm_clflush(address);
	}
}