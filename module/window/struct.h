#pragma once
#include "instance.h"
#include "handle.h"
#include "library/memory.h"
#include <Windows.h>

namespace window {
	class _struct final {
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
		};
		WNDSTRUCTEXW _wsex;
	public:
		inline explicit _struct(void) noexcept
			: _wsex{} {
		};
		inline explicit _struct(_struct const&) noexcept = delete;
		inline explicit _struct(_struct&&) noexcept = delete;
		inline auto operator=(_struct const&) noexcept -> _struct & = delete;
		inline auto operator=(_struct&&) noexcept -> _struct & = delete;
		inline ~_struct(void) noexcept = default;

		inline void create_window(window::handle& handle) noexcept {
			::CreateWindowExW(
				_wsex.dwExStyle, _wsex.lpClassName, _wsex.lpWindowName, _wsex.dwStyle,
				_wsex.x, _wsex.y, _wsex.nWidth, _wsex.nHeight, _wsex.hWndParent,
				_wsex.hMenu, window::instance::data(), &handle);
		}
		inline void adjust_window_rect(void) noexcept {
			RECT rect{ _wsex.x, _wsex.y, _wsex.x + _wsex.nWidth, _wsex.y + _wsex.nHeight };
			AdjustWindowRect(&rect, _wsex.dwStyle, nullptr == _wsex.hMenu ? false : true);
			_wsex.x = rect.left;
			_wsex.y = rect.top;
			_wsex.nWidth = rect.right - rect.left;
			_wsex.nHeight = rect.bottom - rect.top;
		}
		inline void extended_style(DWORD const dwExStyle) noexcept {
			_wsex.dwExStyle = dwExStyle;
		}
		inline void class_name(LPCWSTR const lpClassName) noexcept {
			_wsex.lpClassName = lpClassName;
		}
		inline void window_name(LPCWSTR const lpWindowName) noexcept {
			_wsex.lpWindowName = lpWindowName;
		}
		inline void style(DWORD const dwStyle) noexcept {
			_wsex.dwStyle = dwStyle;
		}
		inline void x(int const x) noexcept {
			_wsex.x = x;
		}
		inline void y(int const y) noexcept {
			_wsex.y = y;
		}
		inline void width(int const nWidth) noexcept {
			_wsex.nWidth = nWidth;
		}
		inline void height(int const nHeight) noexcept {
			_wsex.nHeight = nHeight;
		}
		//_wsex.hWndParent = nullptr;
		inline void menu(HMENU const hMenu) noexcept {
			_wsex.hMenu = hMenu;
		}
	};
}