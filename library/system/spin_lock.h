#pragma once
#include "interlock.h"

namespace libray {
	class spin_lock final {
		unsigned char _lock = 0;
	public:
		inline explicit spin_lock(void) noexcept 
			: _lock(0) {
		};
		inline explicit spin_lock(spin_lock const& rhs) noexcept = delete;
		inline explicit spin_lock(spin_lock&& rhs) noexcept = delete;
		inline auto operator=(spin_lock const& rhs) noexcept -> spin_lock & = delete;
		inline auto operator=(spin_lock&& rhs) noexcept -> spin_lock & = delete;
		inline ~spin_lock(void) noexcept = default;

		inline void lock(void) noexcept {
			while (1 == _lock || 1 == library::interlock_exchange(_lock, 1))
				YieldProcessor();
		}
		inline void unlock(void) noexcept {
			library::interlock_exchange(_lock, 0);
			//_lock = 0;
		}
	};
}