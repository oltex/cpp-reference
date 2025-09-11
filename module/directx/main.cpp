#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>

#include "module/window/instance.h"
#include "module/window/class.h"
#include "module/window/struct.h"

//#include "d3d11/device.h"
#include "dxgi/device.h"


int __stdcall wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prev_hinstance, _In_ LPWSTR command_line, _In_ int command_show) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	window::instance::construct(hinstance);
	//d3d11::device::instance();
	window::handle handle;
	{
		window::_class _class;
		_class.class_name(L"window");
		_class.style(CS_HREDRAW | CS_VREDRAW);
		_class.cursor(window::load_cursor(IDC_ARROW));
		_class.regist();

		window::_struct _struct;
		_struct.class_name(L"window");
		_struct.style(WS_OVERLAPPEDWINDOW);
		_struct.x(CW_USEDEFAULT);
		_struct.width(CW_USEDEFAULT);
		_struct.create_window(handle);
	}
	handle.show(true);
	handle.update();

	for (;;) {
		while (auto msg = window::peek_message()) {
			if (WM_QUIT == msg->message)
				return 0;
			else {
				window::translate_message(*msg);
				window::dispatch_message(*msg);
			}
		}
	}
	return 0;
}