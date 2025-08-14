#pragma once
#include <intrin.h>

namespace library {
	inline void read_barrier(void) noexcept {
		_ReadBarrier();
	}
	inline void write_barrier(void) noexcept {
		_WriteBarrier();
	}
	inline void read_write_barrier(void) noexcept {
		_ReadWriteBarrier();
	}
}