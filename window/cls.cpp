#include "cls.h"
#include "cursor.h"
#include "icon.h"

namespace window {
	cls::cls(void) noexcept {
		initialize();
	}
	void cls::initialize(void) noexcept {
		memset(&_wcex, 0, sizeof(WNDCLASSEX));
		_wcex.cbSize = sizeof(WNDCLASSEX);
		_wcex.lpfnWndProc = DefWindowProcW;
		_wcex.hbrBackground = 0;// reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	}
	auto cls::register_(void) noexcept -> ATOM {
		return RegisterClassExW(&_wcex);
	}
	void cls::set_icon(window::icon const& icon) noexcept {
		_wcex.hIcon = icon._hicon;
	}
	void cls::set_cursor(window::cursor const& cursor) noexcept {
		_wcex.hCursor = cursor._hcursor;
	}
	void cls::set_icon_small(window::icon const& icon) noexcept {
		_wcex.hIconSm = icon._hicon;
	}
}