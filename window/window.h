#pragma once
#include "icon.h"
#include "cursor.h"

namespace window {
	inline auto message(void) noexcept -> int {
		MSG msg;
		while (GetMessageW(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		return (int)msg.wParam;
	};
}
namespace window {
	class registry final {
	public:
		explicit registry(void) noexcept;
		~registry(void) noexcept = default;
	public:
		void initialize(void) noexcept;
		auto register_(void) noexcept -> ATOM;
	public:
		inline void set_style(UINT style) noexcept {
			_wcex.style = style;
		}
		inline void set_procedure(WNDPROC const lpfnWndProc) noexcept {
			_wcex.lpfnWndProc = lpfnWndProc;
		}
		inline void set_class_extra(int const cbClsExtra) noexcept {
			_wcex.cbClsExtra = cbClsExtra;
		}
		inline void set_window_extra(int const cbWndExtra) noexcept {
			_wcex.cbWndExtra = cbWndExtra;
		}
		inline void set_instance(HINSTANCE const instance) noexcept {
			_wcex.hInstance = instance;
		}
		inline void set_icon(window::icon const icon) noexcept {
			_wcex.hIcon = icon._icon;
		}
		inline void set_cursor(window::cursor const cursor) noexcept {
			_wcex.hCursor = cursor._cursor;
		}
		inline void set_background(HBRUSH const hbrBackground) noexcept {
			_wcex.hbrBackground = hbrBackground;
		}
		inline void set_menu_name(LPCWSTR const lpszMenuName) noexcept {
			_wcex.lpszMenuName = lpszMenuName;
		}
		inline void set_class_name(LPCWSTR const lpszClassName) noexcept {
			_wcex.lpszClassName = lpszClassName;
		};
		inline void set_icon_small(window::icon const icon) noexcept {
			_wcex.hIconSm = icon._icon;
		}
	private:
		WNDCLASSEXW _wcex;
	};
}
namespace window {
	class handle;
	class creator final {
		friend handle;
	public:
		struct WNDSTRUCTEXW {
			DWORD dwExStyle;
			LPCWSTR lpClassName;
			LPCWSTR lpWindowName;
			DWORD dwStyle;
			int x;
			int y;
			int nWidth;
			int nHeight;
			HWND hWndParent;
			HMENU hMenu;
			HINSTANCE hInstance;
			LPVOID lpParam;
		};
	public:
		explicit creator(void) noexcept;
		~creator(void) noexcept = default;
	public:
		void initialize(void) noexcept;
	public:
		inline void set_extended_style(DWORD const dwExStyle) noexcept {
			_wsex.dwExStyle = dwExStyle;
		}
		inline void set_class_name(LPCWSTR const lpClassName) noexcept {
			_wsex.lpClassName = lpClassName;
		}
		inline void set_window_name(LPCWSTR const lpWindowName) noexcept {
			_wsex.lpWindowName = lpWindowName;
		}
		inline void set_style(DWORD const dwStyle) noexcept {
			_wsex.dwStyle = dwStyle;
		}
		inline void set_x(int const x) noexcept {
			_wsex.x = x;
		}
		inline void set_y(int const y) noexcept {
			_wsex.y = y;
		}
		inline void set_width(int const nWidth) noexcept {
			_wsex.nWidth = nWidth;
		}
		inline void set_height(int const nHeight) noexcept {
			_wsex.nHeight = nHeight;
		}
		//_wsex.hWndParent = nullptr;
		inline void set_menu(HMENU const hMenu) noexcept {
			_wsex.hMenu = hMenu;
		}
		inline void set_instance(HINSTANCE const instance) noexcept {
			_wsex.hInstance = instance;
		}
		//_wsex.lpParam = nullptr;
	private:
		WNDSTRUCTEXW _wsex;
	};
}
namespace window {
	class handle {
	public:
		explicit handle(creator const& maker) noexcept;
		virtual ~handle(void) noexcept;
	public:
		inline auto show(int const nCmdShow) const noexcept -> BOOL {
			return ShowWindow(_wnd, nCmdShow);
		}
		inline auto move(int const x, int const y, int const width, int const height) const noexcept -> BOOL {
			return MoveWindow(_wnd, x, y, width, height, false);
		}
		inline auto update(void) const noexcept -> BOOL {
			return UpdateWindow(_wnd);
		}
		inline auto close(void) const noexcept {
			CloseWindow(_wnd);
		}
		inline auto destroy(void) const noexcept -> BOOL {
			return DestroyWindow(_wnd);
		}
	protected:
		HWND _wnd = nullptr;
	};
}



//static LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;