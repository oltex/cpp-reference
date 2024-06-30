#include "wnd.h"
#include "sct.h"

namespace window {
	wnd::wnd(sct const& sct_) noexcept {
		_hwnd = CreateWindowExW(
			sct_._wsex.dwExStyle, sct_._wsex.lpClassName, sct_._wsex.lpWindowName, sct_._wsex.dwStyle,
			sct_._wsex.x, sct_._wsex.y, sct_._wsex.nWidth, sct_._wsex.nHeight, sct_._wsex.hWndParent,
			sct_._wsex.hMenu, sct_._wsex.hInstance, sct_._wsex.lpParam);
	}
}