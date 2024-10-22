#pragma once
#include <Windows.h>

namespace thread {
	class spin_lock final {
	public:
		inline explicit spin_lock(void) noexcept = default;
		inline explicit spin_lock(spin_lock const& rhs) noexcept = delete;
		inline explicit spin_lock(spin_lock&& rhs) noexcept = delete;
		inline auto operator=(spin_lock const& rhs) noexcept -> spin_lock & = delete;
		inline auto operator=(spin_lock&& rhs) noexcept -> spin_lock & = delete;
		inline ~spin_lock(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			while (1 == _lock || 1 == _InterlockedExchange(&_lock, 1))
				YieldProcessor();
		}
		inline void unlock(void) noexcept {
			_lock = 0;
		}
	public:
		volatile long _lock = 0;
	};
}