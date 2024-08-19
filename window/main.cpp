#include "window.h"
#include "cls.h"
#include "sct.h"

#include "pen.h"

#include <Windows.h>

window::window* _wnd;
window::device_context* _dc;
window::bitmap* _bitmap;

LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	{
		window::cursor cursor_;
		cursor_.load(nullptr, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

		window::cls cls;
		cls.set_instance(hInstance);
		cls.set_class_name(L"window");
		cls.set_procedure(procedure);
		cls.set_style(CS_HREDRAW | CS_VREDRAW);
		cls.set_cursor(cursor_);
		cls.register_();

		window::sct sct;
		sct.set_instance(hInstance);
		sct.set_class_name(L"window");
		sct.set_style(WS_OVERLAPPEDWINDOW);
		sct.set_x(CW_USEDEFAULT);
		sct.set_width(CW_USEDEFAULT);

		_wnd = new window::window(sct.create());
	}

	window::device_context dc = _wnd->get_device_context();
	RECT rect = _wnd->get_client_rect();

	
	//_dc = new window::device_context(dc.create_compatible_device_context());
	//_bitmap = new window::bitmap(dc.create_compatible_bitmap(rect.right, rect.bottom));
	//_dc->select_object(*_bitmap);

	_wnd->show(true);
	_wnd->update();

	return window::message();
}

LRESULT CALLBACK procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	switch (message) {
	case WM_PAINT: {
		_dc->pat_blt(0, 0, 100, 100, WHITENESS);
		window::pen pen(PS_SOLID, 2, RGB(255, 0, 0));
		_dc->select_object(pen);
		_dc->move_to(0, 0);
		_dc->line_to(100, 100);
		_dc->select_object(*_bitmap);


		window::device_context dc = _wnd->begin_paint();

		dc.bit_blt(0, 0, 100, 100, *_dc, 0, 0, SRCCOPY);

		_wnd->end_paint(dc);
	}
				 break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}