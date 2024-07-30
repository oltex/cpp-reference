#pragma once
#include <Windows.h>

namespace window {
	class icon;
	class cursor;
	class cls final {
	public:
		inline explicit cls(void) noexcept {
			initialize();
		}
		~cls(void) noexcept = default;
	public:
		inline void initialize(void) noexcept {
			memset(&_wcex, 0, sizeof(WNDCLASSEX));
			_wcex.cbSize = sizeof(WNDCLASSEX);
			_wcex.lpfnWndProc = DefWindowProcW;
			_wcex.hbrBackground = 0;
		}
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
		void set_icon(window::icon const& icon) noexcept;
		void set_cursor(window::cursor const& cursor) noexcept;
		inline void set_background(HBRUSH const hbrBackground) noexcept {
			_wcex.hbrBackground = hbrBackground;
		}
		inline void set_menu_name(LPCWSTR const lpszMenuName) noexcept {
			_wcex.lpszMenuName = lpszMenuName;
		}
		inline void set_class_name(LPCWSTR const lpszClassName) noexcept {
			_wcex.lpszClassName = lpszClassName;
		};
		void set_icon_small(window::icon const& icon) noexcept;
	private:
		WNDCLASSEXW _wcex;
	};
}