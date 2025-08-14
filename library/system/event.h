#pragma once
#include "handle.h"

namespace library {
	class event final : public handle {
	public:
		inline explicit event(void) noexcept = default;
		inline explicit event(bool const manual, bool const initial_state) noexcept
			: handle(CreateEventW(nullptr, manual, initial_state, nullptr)) {
		};
		inline explicit event(event const&) noexcept = delete;
		inline explicit event(event&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(event const&) noexcept -> event & = delete;
		inline auto operator=(event&& rhs) noexcept -> event& {
			handle::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~event(void) noexcept override = default;

		inline void create(bool const manual, bool const initial_state) noexcept {
			_handle = CreateEventW(nullptr, manual, initial_state, nullptr);
		}
		inline void set(void) noexcept {
			SetEvent(_handle);
		}
		inline void reset(void) noexcept {
			ResetEvent(_handle);
		}
		inline void pulse(void) noexcept {
			PulseEvent(_handle);
		}
	};
}