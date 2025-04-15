#pragma once
#include <Windows.h>

namespace library::system::multi::lock {
	class spin final {
	public:
		inline explicit spin(void) noexcept = default;
		inline explicit spin(spin const& rhs) noexcept = delete;
		inline explicit spin(spin&& rhs) noexcept = delete;
		inline auto operator=(spin const& rhs) noexcept -> spin & = delete;
		inline auto operator=(spin&& rhs) noexcept -> spin & = delete;
		inline ~spin(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			while (1 == _InterlockedExchange(&_lock, 1))
				while (1 == _lock)
					YieldProcessor();
		}
		inline void unlock(void) noexcept {
			_InterlockedExchange(&_lock, 0);
			//_lock = 0;
		}
	public:
		volatile long _lock = 0;
	};
}