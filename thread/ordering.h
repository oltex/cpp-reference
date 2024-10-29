#pragma once
#include <intrin.h>
//#include <Windows.h>

namespace thread {
	namespace barrier {
		inline static void read(void) noexcept {
			_ReadBarrier();
		}
		inline static void write(void) noexcept {
			_WriteBarrier();
		}
		inline static void read_write(void) noexcept {
			_ReadWriteBarrier();
		}
	}
	namespace fence {
		inline static void load(void) noexcept {
			_mm_lfence();
		}
		inline static void store(void) noexcept {
			_mm_sfence();
		}
		inline static void memory(void) noexcept {
			_mm_mfence();
		}
		inline static void fast_store(void) noexcept {
			__faststorefence();
		}
	}
}