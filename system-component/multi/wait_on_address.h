#pragma once
#pragma comment(lib, "Synchronization.lib")
#include <Windows.h>

namespace system_component::multi {
	class wait_on_address final {
	public:
		inline explicit wait_on_address(void) noexcept = default;
		inline explicit wait_on_address(wait_on_address const& rhs) noexcept = delete;
		inline explicit wait_on_address(wait_on_address&& rhs) noexcept = delete;
		inline auto operator=(wait_on_address const& rhs) noexcept -> wait_on_address & = delete;
		inline auto operator=(wait_on_address&& rhs) noexcept -> wait_on_address & = delete;
		inline ~wait_on_address(void) noexcept = default;
	public:
		inline void wait(unsigned long milli_second) noexcept {
			volatile long compare = 1;
			WaitOnAddress(&_address, (void*)&compare, sizeof(long), milli_second);
		}
		inline void wake_single(void) noexcept {
			WakeByAddressSingle((void*)&_address);
		}
		inline void wake_all(void) noexcept {
			WakeByAddressAll((void*)&_address);
		}
	private:
		volatile long _address;
	};
}