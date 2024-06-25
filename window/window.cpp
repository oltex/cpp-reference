#include "window.h"

namespace window {
	registry::registry(void) noexcept {
		initialize();
	}
	void registry::initialize(void) noexcept {
		memset(&_wcex, 0, sizeof(WNDCLASSEX));
		_wcex.cbSize = sizeof(WNDCLASSEX);
		_wcex.lpfnWndProc = DefWindowProcW;
		_wcex.hbrBackground = 0;// reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	}
	auto registry::register_(void) noexcept -> ATOM {
		return RegisterClassExW(&_wcex);
	}
}

namespace window {
	creator::creator(void) noexcept {
		initialize();
	}
	void creator::initialize(void) noexcept {
		memset(&_wsex, 0, sizeof(WNDSTRUCTEXW));
	}
}

namespace window {
	handle::handle(creator const& creator_) noexcept {
		_wnd = CreateWindowExW(
			creator_._wsex.dwExStyle, creator_._wsex.lpClassName, creator_._wsex.lpWindowName, creator_._wsex.dwStyle,
			creator_._wsex.x, creator_._wsex.y, creator_._wsex.nWidth, creator_._wsex.nHeight, creator_._wsex.hWndParent,
			creator_._wsex.hMenu, creator_._wsex.hInstance, creator_._wsex.lpParam);
	}
	handle::~handle(void) noexcept {
	}
}

LRESULT procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	switch (message)
	{
	case WM_PAINT:
	{
	    PAINTSTRUCT ps;
	    HDC hdc = BeginPaint(wnd, &ps);
	    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	    EndPaint(wnd, &ps);
	}
	break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(wnd, message, wparam, lparam);
	}
	return 0;
}
