#include "cls.h"
#include "cursor.h"
#include "icon.h"

namespace window {
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