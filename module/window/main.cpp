#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "window.h"
#include "cls.h"
#include "sct.h"

auto message(void) noexcept -> MSG;
LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;
int APIENTRY wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	window::instance instance(hinstance);
	window::cursor cursor;
	cursor.load(nullptr, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	window::cls cls;
	cls.set_instance(instance);
	cls.set_class_name(L"window");
	cls.set_procedure(procedure);
	cls.set_style(CS_HREDRAW | CS_VREDRAW);
	cls.set_cursor(cursor);
	cls.register_();

	window::sct sct;
	sct.set_instance(instance);
	sct.set_class_name(L"window");
	sct.set_style(WS_OVERLAPPEDWINDOW);
	sct.set_x(CW_USEDEFAULT);
	sct.set_width(CW_USEDEFAULT);

	window::window window = sct.create();
	window.show(true);
	window.update();
	MSG msg = message();
	return static_cast<int>(msg.wParam);
}

auto message(void) noexcept -> MSG {
	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return msg;
};

LRESULT CALLBACK procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}