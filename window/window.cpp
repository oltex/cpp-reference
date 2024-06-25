#include "window.h"

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
}

namespace window {
	creator::creator(void) noexcept {
		initialize();
	}
	void creator::initialize(void) noexcept {
		memset(&_wsex, 0, sizeof(WNDSTRUCTEXW));
	}
}