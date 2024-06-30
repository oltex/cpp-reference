#include <Windows.h>
#include "window.h"

using namespace window;
wnd* _wnd;
dc* _dc;
bitmap* _bitmap;

LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	{
		cursor cursor_;
		cursor_.load(nullptr, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

		cls cls;
		cls.set_instance(hInstance);
		cls.set_class_name(L"window");
		cls.set_procedure(procedure);
		cls.set_style(CS_HREDRAW | CS_VREDRAW);
		cls.set_cursor(cursor_);
		cls.register_();

		sct sct;
		sct.set_instance(hInstance);
		sct.set_class_name(L"window");
		sct.set_style(WS_OVERLAPPEDWINDOW);
		sct.set_x(CW_USEDEFAULT);
		sct.set_width(CW_USEDEFAULT);

		_wnd = new wnd(sct);
	}

	dc get_dc(*_wnd, window::dc::flag::get_dc);
	RECT rect = _wnd->get_client_rect();

	_dc = new window::dc(get_dc);
	_bitmap = new window::bitmap(get_dc, rect.right, rect.bottom);
	_dc->select_object(*_bitmap);

	_wnd->show(true);
	_wnd->update();

	return window::message();
}

LRESULT CALLBACK procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	switch (message) {
	case WM_CREATE: {

	}
				  break;
	case WM_PAINT: {
		_dc->pat_blt(0, 0, 100, 100, WHITENESS);
		pen pen(PS_SOLID, 2, RGB(255, 0, 0));
		_dc->select_object(pen);
		_dc->move_to(0, 0);
		_dc->line_to(100, 100);
		_dc->select_object(*_bitmap);

		dc dc_(*_wnd, window::dc::flag::begin_paint);
		dc_.bit_blt(0, 0, 100, 100, *_dc, 0, 0, SRCCOPY);
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