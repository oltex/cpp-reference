#include "window.h"

namespace framework {
	window::window(void) noexcept {
		_thread.begin(&window::execute, 0, this);
		while (nullptr == _hwnd) {}
	}

	bool window::is_exit(void) noexcept {
		return WAIT_OBJECT_0 == _thread.wait_for_single(0);
	}
	void window::execute(void) noexcept {
		winapi::_class _class;
		_class.class_name(L"window");
		_class.style(CS_HREDRAW | CS_VREDRAW);
		_class.cursor(::winapi::load_cursor(IDC_ARROW));
		_class.regist();
		winapi::_struct _struct;
		_struct.class_name(L"window");
		_struct.style(WS_OVERLAPPEDWINDOW);
		_struct.x(CW_USEDEFAULT);
		_struct.width(CW_USEDEFAULT);
		_struct.create_window(*this);
		show(true);

		while (auto msg = winapi::get_message()) {
			winapi::translate_message(*msg);
			winapi::dispatch_message(*msg);
		}
	}
}