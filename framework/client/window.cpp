#include "window.h"
#include "editor.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace framework {
	window::window(void) noexcept {
		_thread.begin(&window::execute, 0, this);
		while (nullptr == _hwnd) {}
	}

	bool window::check_exit(void) noexcept {
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
	bool window::procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
		//if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam))
			//return true;
		return false;
	}
}