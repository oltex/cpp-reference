#pragma once
#pragma comment(lib, "Synchronization.lib")
#include <Windows.h>

namespace thread {
	class wait_on_address_lock final {
	public:
		inline explicit wait_on_address_lock(void) noexcept = default;
		inline explicit wait_on_address_lock(wait_on_address_lock const& rhs) noexcept = delete;
		inline explicit wait_on_address_lock(wait_on_address_lock&& rhs) noexcept = delete;
		inline auto operator=(wait_on_address_lock const& rhs) noexcept -> wait_on_address_lock & = delete;
		inline auto operator=(wait_on_address_lock&& rhs) noexcept -> wait_on_address_lock & = delete;
		inline ~wait_on_address_lock(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			volatile long compare = 1;
			while (1 == _lock || 1 == _InterlockedExchange(&_lock, 1))
				WaitOnAddress(&_lock, (void*)&compare, sizeof(long), INFINITE);
		}
		inline void unlock(void) noexcept {
			_lock = 0;
			WakeByAddressSingle((void*)&_lock);
		}
	private:
		volatile long _lock = 0;
	};
}