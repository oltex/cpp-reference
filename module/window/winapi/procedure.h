#pragma once
#include "handle.h"
#include <Windows.h>
#include <optional>

namespace winapi {
	inline auto get_message(void) noexcept -> std::optional<MSG> {
		MSG msg;
		if (!::GetMessageW(&msg, nullptr, 0, 0))
			return std::nullopt;
		else
			return msg;
	};
	inline auto peek_message(void) noexcept -> std::optional<MSG> {
		MSG msg;
		if (!::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
			return std::nullopt;
		else
			return msg;
	}
	inline void translate_message(MSG& msg) noexcept {
		::TranslateMessage(&msg);
	}
	inline void dispatch_message(MSG& msg) noexcept {
		::DispatchMessageW(&msg);
	}
	inline void post_quit_message(int const exit_code) noexcept {
		::PostQuitMessage(exit_code);
	}


	inline auto __stdcall procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept -> LRESULT {
		auto handle = reinterpret_cast<winapi::handle*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		if (nullptr != handle && handle->procedure(hwnd, message, wparam, lparam))
			return true;
		switch (message) {
		case WM_NCCREATE: {
			auto& _struct = *reinterpret_cast<CREATESTRUCT*>(lparam);
			auto& handle = *reinterpret_cast<winapi::handle*>(_struct.lpCreateParams);
			handle.data() = hwnd;
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&handle));
		} break;
		//case WM_MOUSEMOVE:
		//	//handle->mouse_move(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_MOUSEWHEEL:
		//	break;
		//case WM_LBUTTONDOWN:
		//	//handle->left_butten_down(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_LBUTTONUP:
		//	//handle->left_butten_up(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_RBUTTONDOWN:
		//	//handle->right_butten_down(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_RBUTTONUP:
		//	//handle->right_butten_up(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_MBUTTONDOWN:
		//	//handle->middle_butten_down(LOWORD(lparam), HIWORD(lparam));
		//	break;
		//case WM_MBUTTONUP:
		//	//handle->middle_butten_up(LOWORD(lparam), HIWORD(lparam));
		//	break;
		////case WM_KEYDOWN:
		////case WM_TIMER:
		////case WM_PAINT:
		//	break;
		case WM_DESTROY:
			handle->destory();
			post_quit_message(0);
			break;
		}
		return DefWindowProcW(hwnd, message, wparam, lparam);
	}
}