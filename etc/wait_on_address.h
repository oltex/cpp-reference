#pragma once
#pragma comment(lib, "Synchronization.lib")
#include <Windows.h>

namespace library::system::multi::lock {
	class wait_on_address final {
	public:
		inline explicit wait_on_address(void) noexcept = default;
		inline explicit wait_on_address(wait_on_address const& rhs) noexcept = delete;
		inline explicit wait_on_address(wait_on_address&& rhs) noexcept = delete;
		inline auto operator=(wait_on_address const& rhs) noexcept -> wait_on_address & = delete;
		inline auto operator=(wait_on_address&& rhs) noexcept -> wait_on_address & = delete;
		inline ~wait_on_address(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			volatile long compare = 1;
			while (1 == _address || 1 == _InterlockedExchange(&_address, 1))
				WaitOnAddress(&_address, (void*)&compare, sizeof(long), INFINITE);
		}
		inline void unlock(void) noexcept {
			_address = 0;
			WakeByAddressSingle((void*)&_address);
		}
	private:
		volatile long _address = 0;
	};
}