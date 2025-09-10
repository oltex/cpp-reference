#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "instance.h"
#include "class.h"
#include "struct.h"
#include <Windows.h>

int __stdcall wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prev_hinstance, _In_ LPWSTR command_line, _In_ int command_show) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	window::instance::construct(hinstance);
	window::cursor cursor(window::load_image(IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));

	window::_class _class;
	_class.class_name(L"window");
	_class.style(CS_HREDRAW | CS_VREDRAW);
	_class.cursor(cursor);
	_class.regist();

	window::_struct _struct;
	_struct.class_name(L"window");
	_struct.style(WS_OVERLAPPEDWINDOW);
	_struct.x(CW_USEDEFAULT);
	_struct.width(CW_USEDEFAULT);

	window::handle handle;
	_struct.create_window(handle);

	handle.show(true);
	while (auto msg = window::get_message()) {
		window::translate_message(*msg);
		window::dispatch_message(*msg);
	}
	//return static_cast<int>(msg.wParam);
	return 0;
}