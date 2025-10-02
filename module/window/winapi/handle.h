#pragma once
#include "library/function.h"
#include "library/container/pair.h"
#include "device_context.h"
#include <Windows.h>

namespace winapi {
	class handle {
	protected:
		HWND _hwnd;
	public:
		inline explicit handle(void) noexcept
			: _hwnd(nullptr) {
		}
		inline explicit handle(handle const&) noexcept = delete;
		inline explicit handle(handle&& rhs) noexcept
			: _hwnd(library::exchange(rhs._hwnd, nullptr)) {
		}
		inline auto operator=(handle const&) noexcept -> handle & = delete;
		inline auto operator=(handle&& rhs) noexcept -> handle& {
			if (nullptr != _hwnd)
				::DestroyWindow(_hwnd);
			_hwnd = library::exchange(rhs._hwnd, nullptr);
			return *this;
		};
		inline ~handle(void) noexcept {
			if (nullptr != _hwnd)
				::DestroyWindow(_hwnd);
		};

		inline auto get_device_context(void) const noexcept -> device_context {
			HDC hdc = ::GetDC(_hwnd);
			return device_context(hdc);
		}
		inline void release_device_context(device_context& dc) const noexcept {
			::ReleaseDC(_hwnd, dc.data());
			dc.data() = nullptr;
		}
		inline auto begin_paint(void) const noexcept -> library::pair<device_context, PAINTSTRUCT> {
			PAINTSTRUCT paint_struct;
			HDC hdc = ::BeginPaint(_hwnd, &paint_struct);
			return library::pair<device_context, PAINTSTRUCT>(device_context(hdc), paint_struct);
		}
		inline void end_paint(device_context& dc, PAINTSTRUCT& paint_struct) const noexcept {
			::EndPaint(_hwnd, &paint_struct);
			dc.data() = nullptr;
		}
		inline void set_timer(UINT_PTR IdEvent, UINT elapse, TIMERPROC func) noexcept {
			::SetTimer(_hwnd, IdEvent, elapse, func);
		}
		inline void kill_timer(UINT_PTR IdEvent) noexcept {
			::KillTimer(_hwnd, IdEvent);
		}
		inline auto screen_to_client(POINT point) const noexcept -> POINT {
			::ScreenToClient(_hwnd, &point);
			return point;
		}
		inline auto show(int const nCmdShow) const noexcept -> bool {
			return ::ShowWindow(_hwnd, nCmdShow);
		}
		inline auto move(int const x, int const y, int const width, int const height) const noexcept -> bool {
			return ::MoveWindow(_hwnd, x, y, width, height, false);
		}
		inline auto update(void) const noexcept -> bool {
			return ::UpdateWindow(_hwnd);
		}
		inline auto close(void) const noexcept -> bool {
			return ::CloseWindow(_hwnd);
		}
		inline auto get_client_rect(void) const noexcept -> RECT {
			RECT rect;
			::GetClientRect(_hwnd, &rect);
			return rect;
		}
		inline auto get_winapi_rect(void) const noexcept -> RECT {
			RECT rect;
			::GetWindowRect(_hwnd, &rect);
			return rect;
		}
		inline auto set_winapi_text(wchar_t const* const string) const noexcept -> BOOL {
			return ::SetWindowTextW(_hwnd, string);
		}
		inline auto invalidate_rect(RECT const* rect, bool const erase) const noexcept -> bool {
			return ::InvalidateRect(_hwnd, rect, erase);
		}

		inline bool virtual procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
			return false;
		};
		inline void virtual mouse_move(int const x, int const y) noexcept {}
		inline void virtual left_butten_down(int const x, int const y) noexcept {}
		inline void virtual left_butten_up(int const x, int const y) noexcept {}
		inline void virtual right_butten_down(int const x, int const y) noexcept {}
		inline void virtual right_butten_up(int const x, int const y) noexcept {}
		inline void virtual middle_butten_down(int const x, int const y) noexcept {}
		inline void virtual middle_butten_up(int const x, int const y) noexcept {}
		inline void virtual destory(void) noexcept {}

		inline auto data(void) noexcept -> HWND& {
			return _hwnd;
		}
	};
}