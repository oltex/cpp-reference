#pragma once
#include "instance.h"
#include "icon.h"
#include "cursor.h"
#include "handle.h"
#include <Windows.h>

namespace window {
	class _class final {
		WNDCLASSEXW _wndclassex;
	public:
		inline explicit _class(void) noexcept
			: _wndclassex{ .cbSize = sizeof(WNDCLASSEX), .lpfnWndProc = window::procedure, .hInstance = window::instance::data() } {
		};
		inline explicit _class(_class const&) noexcept = delete;
		inline explicit _class(_class&&) noexcept = delete;
		inline auto operator=(_class const&) noexcept -> _class & = delete;
		inline auto operator=(_class&&) noexcept -> _class & = delete;
		inline ~_class(void) noexcept = default;

		inline void regist(void) const noexcept {
			::RegisterClassExW(&_wndclassex);
		}
		//inline static void unregist(void) const noexcept {
		//	UnregisterClassW(_wndclassex.lpszClassName, _wndclassex.hInstance);
		//}

		inline void style(unsigned int style) noexcept {
			_wndclassex.style = style;
		}
		inline void procedure(WNDPROC const lpfnWndProc) noexcept {
			_wndclassex.lpfnWndProc = lpfnWndProc;
		}
		inline void class_extra(int const cbClsExtra) noexcept {
			_wndclassex.cbClsExtra = cbClsExtra;
		}
		inline void window_extra(int const cbWndExtra) noexcept {
			_wndclassex.cbWndExtra = cbWndExtra;
		}
		inline void instance(instance const& instance) noexcept {
			_wndclassex.hInstance = instance.data();
		}
		inline void icon(window::icon const& icon) noexcept {
			_wndclassex.hIcon = icon.data();
		}
		inline void cursor(window::cursor const& cursor) noexcept {
			_wndclassex.hCursor = cursor.data();
		}
		inline void background(HBRUSH const hbrBackground) noexcept {
			_wndclassex.hbrBackground = hbrBackground;
		}
		inline void menu_name(wchar_t const* const lpszMenuName) noexcept {
			_wndclassex.lpszMenuName = lpszMenuName;
		}
		inline void class_name(wchar_t const* const lpszClassName) noexcept {
			_wndclassex.lpszClassName = lpszClassName;
		};
		inline void icon_small(window::icon const& icon) noexcept {
			_wndclassex.hIconSm = icon.data();
		}
	};
}
//_wcex.lpfnWndProc = DefWindowProcW;