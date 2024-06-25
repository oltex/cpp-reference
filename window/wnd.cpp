#include "wnd.h"
#include "window.h"

namespace window {
	wnd::wnd(creator const& creator_) noexcept {
		_hwnd = CreateWindowExW(
			creator_._wsex.dwExStyle, creator_._wsex.lpClassName, creator_._wsex.lpWindowName, creator_._wsex.dwStyle,
			creator_._wsex.x, creator_._wsex.y, creator_._wsex.nWidth, creator_._wsex.nHeight, creator_._wsex.hWndParent,
			creator_._wsex.hMenu, creator_._wsex.hInstance, creator_._wsex.lpParam);
	}
}