#pragma once
#include <Windows.h>

namespace winapi {
	class cursor final {
		HCURSOR _hcursor;
		bool _share;
	public:
		inline explicit cursor(void) noexcept
			: _hcursor(GetCursor()), _share(true) {
		};
		inline explicit cursor(HANDLE handle) noexcept
			: _hcursor(static_cast<HCURSOR>(handle)), _share(true) {
		};
		inline explicit cursor(HCURSOR handle) noexcept
			: _hcursor(handle), _share(true) {
		};
		inline explicit cursor(cursor const&) noexcept = delete;
		inline explicit cursor(cursor&&) noexcept = delete;
		inline auto operator=(cursor const&) noexcept -> cursor & = delete;
		inline auto operator=(cursor&&) noexcept -> cursor & = delete;
		inline ~cursor(void) noexcept {
			if (nullptr != _hcursor && false == _share)
				::DestroyCursor(_hcursor);
		};

		static inline auto clip(RECT const rect) noexcept -> BOOL {
			return ::ClipCursor(&rect);
		}
		static inline auto set_position(int const x, int const y) -> bool {
			return ::SetCursorPos(x, y);
		}
		static inline auto show(bool const show) noexcept -> int {
			return ::ShowCursor(show);
		};
		inline auto data(void) const noexcept -> HCURSOR {
			return _hcursor;
		}
	};

	inline static auto load_cursor(wchar_t const* const name) noexcept -> cursor {
		return cursor(::LoadCursorW(nullptr, name));
	}
}