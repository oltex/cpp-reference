#pragma once
#include <Windows.h>

namespace window {
	namespace cursor_n {
		inline auto clip(RECT const rect) noexcept -> BOOL {
			return ClipCursor(&rect);
		}
		inline auto set_pos(int const x, int const y) -> BOOL {
			return SetCursorPos(x, y);
		}
		inline auto show(BOOL const bShow) noexcept -> int {
			return ShowCursor(bShow);
		};
	}
	class registry;
	class cursor final {
		friend class registry;
	public:
		explicit cursor(void) noexcept = default;
		~cursor(void) noexcept = default;
	public:
		inline auto destroy(void) noexcept -> BOOL {
			return DestroyCursor(_cursor);
		}
		inline void get(void) noexcept {
			_cursor = GetCursor();
		}
		inline void load(HINSTANCE const hInst, LPCWSTR const name, UINT const type, int const cx, int const cy, UINT const fuLoad) noexcept {
			_cursor = static_cast<HCURSOR>(LoadImageW(hInst, name, type, cx, cy, fuLoad));
			DWORD a = GetLastError();
		};
	private:
		HCURSOR _cursor = nullptr;
	};
}