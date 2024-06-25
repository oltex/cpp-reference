#include <Windows.h>
#include "window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	window::cursor cursor_;
	cursor_.load(nullptr, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	window::registry registry;
	registry.set_instance(hInstance);
	registry.set_class_name(L"window");
	registry.set_style(CS_HREDRAW | CS_VREDRAW);
	registry.set_cursor(cursor_);
	registry.register_();
	 
	window::creator creator;
	creator.set_instance(hInstance);
	creator.set_class_name(L"window");
	creator.set_style(WS_OVERLAPPEDWINDOW);
	creator.set_x(CW_USEDEFAULT);
	creator.set_width(CW_USEDEFAULT);

	window::handle handle(creator);
	handle.show(true);
	handle.update();

	return window::message();
}