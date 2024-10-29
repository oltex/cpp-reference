#pragma once
#include <Windows.h>

namespace thread {
	class event final {
		inline explicit event(bool const manual, bool const initial_state) noexcept
			: _event(CreateEventW(nullptr, manual, initial_state, nullptr)) {
		};
		inline explicit event(event const& rhs) noexcept = delete;
		inline explicit event(event&& rhs) noexcept = delete;
		inline auto operator=(event const& rhs) noexcept -> event & = delete;
		inline auto operator=(event&& rhs) noexcept -> event & = delete;
		inline ~event(void) noexcept {
			CloseHandle(_event);
		};
	public:
		inline void set(void) noexcept {
			SetEvent(_event);
		}
		inline void reset(void) noexcept {
			ResetEvent(_event);
		}
		inline void pulse(void) noexcept {
			PulseEvent(_event);
		}
		inline auto wait(unsigned long milli_second) noexcept -> unsigned long {
			return WaitForSingleObject(_event, milli_second);
		}
	private:
		HANDLE _event;
	};
}