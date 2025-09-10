#pragma once
#include <Windows.h>
#include "library/function.h"

namespace window {
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
				DestroyWindow(_hwnd);
			_hwnd = library::exchange(rhs._hwnd, nullptr);
			return *this;
		};
		inline ~handle(void) noexcept {
			if (nullptr != _hwnd)
				::DestroyWindow(_hwnd);
		};

		//inline auto get_device_context(void) const noexcept -> device_context {
		//	HDC hdc = GetDC(_hwnd);
		//	return device_context(hdc);
		//}
		//inline void release_device_context(device_context& dc) const noexcept {
		//	ReleaseDC(_hwnd, dc.data());
		//}
		//inline auto begin_paint(void) const noexcept -> device_context {
		//	PAINTSTRUCT paintstruct;
		//	HDC hdc = BeginPaint(_hwnd, &paintstruct);
		//	return device_context(hdc, paintstruct);
		//}
		//inline void end_paint(device_context& dc) const noexcept {
		//	EndPaint(_hwnd, &dc.get_paint_struct());
		//}
		//inline void set_timer(UINT_PTR IdEvent, UINT Elapse, TIMERPROC lpTimerFunc) noexcept {
		//	SetTimer(_hwnd, IdEvent, Elapse, lpTimerFunc);
		//}
		//inline void kill_timer(UINT_PTR IdEvent) noexcept {
		//	KillTimer(_hwnd, IdEvent);
		//}
		//inline void screen_to_client(LPPOINT const lpPoint) const noexcept {
		//	ScreenToClient(_hwnd, lpPoint);
		//}
		inline auto show(int const nCmdShow) const noexcept -> bool {
			return ::ShowWindow(_hwnd, nCmdShow);
		}
		//inline auto move(int const x, int const y, int const width, int const height) const noexcept -> BOOL {
		//	return MoveWindow(_hwnd, x, y, width, height, false);
		//}
		//inline auto update(void) const noexcept -> BOOL {
		//	return UpdateWindow(_hwnd);
		//}
		//inline auto close(void) const noexcept -> BOOL {
		//	return CloseWindow(_hwnd);
		//}
		//inline auto get_client_rect(void) const noexcept -> RECT {
		//	RECT rect;
		//	GetClientRect(_hwnd, &rect);
		//	return rect;
		//}
		//inline auto get_window_rect(void) const noexcept -> RECT {
		//	RECT rect;
		//	GetWindowRect(_hwnd, &rect);
		//	return rect;
		//}
		//inline auto set_window_text(LPCWSTR const lpString) const noexcept -> BOOL {
		//	return SetWindowTextW(_hwnd, lpString);
		//}
		//inline auto invalidate_rect(RECT const* lpRect, BOOL const bErase) const noexcept -> BOOL {
		//	return InvalidateRect(_hwnd, lpRect, bErase);
		//}



		inline void virtual left_butten_down(int const x, int const y) noexcept {
		}
		inline void virtual left_butten_up(int const x, int const y) noexcept {
		}
		inline void virtual right_butten_down(int const x, int const y) noexcept {
		}
		inline void virtual right_butten_up(int const x, int const y) noexcept {
		}
		inline void virtual middle_butten_down(int const x, int const y) noexcept {
		}
		inline void virtual middle_butten_up(int const x, int const y) noexcept {
		}

		inline auto data(void) noexcept -> HWND& {
			return _hwnd;
		}
	};

	inline auto message(void) noexcept -> MSG {
		MSG msg;
		while (::GetMessageW(&msg, nullptr, 0, 0)) {
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
		return msg;
	};
	inline auto __stdcall procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept -> LRESULT {
		window::handle* handle;
		if (WM_NCCREATE == message) {
			auto& _struct = *reinterpret_cast<CREATESTRUCT*>(lparam);
			auto& handle = *reinterpret_cast<window::handle*>(_struct.lpCreateParams);
			handle.data() = hwnd;
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&handle));
		}
		else
			handle = reinterpret_cast<window::handle*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));

		switch (message) {
		case WM_MOUSEMOVE:
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			hndl->mouse_move(x, y); // 콜백 호출
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDOWN:
			handle->left_butten_down(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_LBUTTONUP:
			handle->left_butten_up(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_RBUTTONDOWN:
			handle->right_butten_down(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_RBUTTONUP:
			handle->right_butten_up(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_MBUTTONDOWN:
			handle->middle_butten_down(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_MBUTTONUP:
			handle->middle_butten_up(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_KEYDOWN:
		case WM_TIMER:
		case WM_PAINT:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
		return 0;
	}
}