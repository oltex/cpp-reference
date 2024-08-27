#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "window/cursor.h"
#include "window/cls.h"
#include "window/sct.h"

#include "engine.h"
#include <iostream>

LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;
int APIENTRY wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prevhinstance, _In_ LPWSTR cmdline, _In_ int cmdshow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
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

		engine::engine::constructor(instance, window);
	}
	engine::engine::instance().update();
	return 0;
}


LRESULT CALLBACK procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}