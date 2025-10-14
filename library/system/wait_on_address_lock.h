#pragma once
#include <Windows.h>

namespace library {
	class wait_on_address_lock final {
		volatile long _address = 0;
	public:
		inline explicit wait_on_address_lock(void) noexcept = default;
		inline explicit wait_on_address_lock(wait_on_address_lock const& rhs) noexcept = delete;
		inline explicit wait_on_address_lock(wait_on_address_lock&& rhs) noexcept = delete;
		inline auto operator=(wait_on_address_lock const& rhs) noexcept -> wait_on_address_lock & = delete;
		inline auto operator=(wait_on_address_lock&& rhs) noexcept -> wait_on_address_lock & = delete;
		inline ~wait_on_address_lock(void) noexcept = default;

		inline void lock(void) noexcept {
			volatile long compare = 1;
			while (1 == _address || 1 == ::_InterlockedExchange(&_address, 1))
				::WaitOnAddress(&_address, (void*)&compare, sizeof(long), INFINITE);
		}
		inline void unlock(void) noexcept {
			_address = 0;
			::WakeByAddressSingle((void*)&_address);
		}
	};
}