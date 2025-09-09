#pragma once
#include "instance.h"
#include "icon.h"
#include "cursor.h"
#include <Windows.h>

namespace window {
	class _class final {
		WNDCLASSEXW _wndclassex;
	public:
		inline explicit _class(void) noexcept
			: _wndclassex{ .cbSize = sizeof(WNDCLASSEX) } {
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
		inline void set_procedure(WNDPROC const lpfnWndProc) noexcept {
			_wndclassex.lpfnWndProc = lpfnWndProc;
		}
		inline void set_class_extra(int const cbClsExtra) noexcept {
			_wndclassex.cbClsExtra = cbClsExtra;
		}
		inline void set_window_extra(int const cbWndExtra) noexcept {
			_wndclassex.cbWndExtra = cbWndExtra;
		}
		inline void set_instance(instance const& instance) noexcept {
			_wndclassex.hInstance = instance.data();
		}
		inline void set_icon(icon const& icon) noexcept {
			_wndclassex.hIcon = icon.data();
		}
		inline void set_cursor(cursor const& cursor) noexcept {
			_wndclassex.hCursor = cursor.data();
		}
		inline void set_background(HBRUSH const hbrBackground) noexcept {
			_wndclassex.hbrBackground = hbrBackground;
		}
		inline void set_menu_name(wchar_t const* const lpszMenuName) noexcept {
			_wndclassex.lpszMenuName = lpszMenuName;
		}
		inline void set_class_name(wchar_t const* const lpszClassName) noexcept {
			_wndclassex.lpszClassName = lpszClassName;
		};
		inline void set_icon_small(icon const& icon) noexcept {
			_wndclassex.hIconSm = icon.data();
		}
	};
}


#pragma once


namespace window {
	class cls final {
	public:
		inline explicit cls(void) noexcept {
			initialize();
		}
		inline ~cls(void) noexcept = default;
	public:
		inline void initialize(void) noexcept {
		
		}

	public:


	};
}

_wcex.lpfnWndProc = DefWindowProcW;
_wcex.hbrBackground = 0;