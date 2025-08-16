#pragma once
#include <Windows.h>

namespace library {
	template <typename type>
	inline bool wait_on_address(type& address, type& compare, unsigned long const milli_second) noexcept {
		static_assert(std::is_trivially_copyable_v<type>, "T must be trivially copyable");
		static_assert(sizeof(type) == 1 || sizeof(type) == 2 || sizeof(type) == 4 || sizeof(type) == 8, "T size must be 1/2/4/8 bytes");
		return WaitOnAddress(reinterpret_cast<void*>(&address), reinterpret_cast<void*>(&compare), sizeof(type), milli_second);
	}
	template <typename type>
	inline void wake_by_address_single(type& address) noexcept {
		WakeByAddressSingle(reinterpret_cast<void*>(&address));
	}
	inline void wake_by_address_all(void* address) noexcept {
		WakeByAddressAll(address);
	}
}