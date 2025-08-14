#pragma once
#include <Windows.h>

namespace library {
	inline bool wait_on_address(void* address, void* _compare, size_t const size, unsigned long const milli_second) noexcept {
		return WaitOnAddress(address, _compare, size, milli_second);
	}
	inline void wake_by_address_single(void* address) noexcept {
		WakeByAddressSingle(address);
	}
	inline void wake_by_address_all(void* address) noexcept {
		WakeByAddressAll(address);
	}
}