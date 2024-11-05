#pragma once
#include "../kernel/object.h"
#include <Windows.h>
#include <utility>

namespace multi {
	class event final : public kernel::object {
	public:
		inline explicit event(void) noexcept = default;
		inline explicit event(bool const manual, bool const initial_state) noexcept
			: object(CreateEventW(nullptr, manual, initial_state, nullptr)) {
		};
		inline explicit event(event const& rhs) noexcept = delete;
		inline explicit event(event&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(event const& rhs) noexcept -> event & = delete;
		inline auto operator=(event&& rhs) noexcept -> event& {
			object::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~event(void) noexcept override = default;
	public:
		inline void set(void) noexcept {
			SetEvent(_handle);
		}
		inline void reset(void) noexcept {
			ResetEvent(_handle);
		}
		inline void pulse(void) noexcept {
			PulseEvent(_handle);
		}
		inline auto wait(unsigned long milli_second) noexcept -> unsigned long {
			return WaitForSingleObject(_handle, milli_second);
		}
	};
}