#pragma once
#include <Windows.h>

namespace window {
	class sct;
	class wnd {
		friend class dc;
	public:
		explicit wnd(sct const& sct_) noexcept;
		virtual ~wnd(void) noexcept = default;
	public:
		inline void screen_to_client(LPPOINT const lpPoint) const noexcept {
			ScreenToClient(_hwnd, lpPoint);
		}
		inline auto show(int const nCmdShow) const noexcept -> BOOL {
			return ShowWindow(_hwnd, nCmdShow);
		}
		inline auto move(int const x, int const y, int const width, int const height) const noexcept -> BOOL {
			return MoveWindow(_hwnd, x, y, width, height, false);
		}
		inline auto update(void) const noexcept -> BOOL {
			return UpdateWindow(_hwnd);
		}
		inline auto close(void) const noexcept -> BOOL {
			return CloseWindow(_hwnd);
		}
		inline auto destroy(void) const noexcept -> BOOL {
			return DestroyWindow(_hwnd);
		}
		inline auto get_client_rect(void) const noexcept -> RECT {
			RECT rect;
			GetClientRect(_hwnd, &rect);
			return rect;
		}
		inline auto get_window_rect(void) const noexcept -> RECT {
			RECT rect;
			GetWindowRect(_hwnd, &rect);
			return rect;
		}
		inline auto set_window_text(LPCWSTR const lpString) const noexcept -> BOOL {
			return SetWindowTextW(_hwnd, lpString);
		}
		inline auto invalidate_rect(RECT const& lpRect, BOOL const bErase) const noexcept -> BOOL {
			return InvalidateRect(_hwnd, &lpRect, bErase);
		}
	protected:
		HWND _hwnd = nullptr;
	};
}