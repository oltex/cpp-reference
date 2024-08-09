#pragma once
#include "wnd.h"
#include <Windows.h>

namespace window {
	class sct final {
	public:
		struct WNDSTRUCTEXW final {
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
		inline explicit sct(void) noexcept {
			initialize();
		}
		~sct(void) noexcept = default;
	public:
		inline void initialize(void) noexcept {
			memset(&_wsex, 0, sizeof(WNDSTRUCTEXW));
		}
		inline auto create(void) noexcept -> wnd {
			HWND hwnd = CreateWindowExW(
				_wsex.dwExStyle, _wsex.lpClassName, _wsex.lpWindowName, _wsex.dwStyle,
				_wsex.x, _wsex.y, _wsex.nWidth, _wsex.nHeight, _wsex.hWndParent,
				_wsex.hMenu, _wsex.hInstance, _wsex.lpParam);
			return wnd(hwnd);
		}
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