#include <Windows.h>
#include "window.h"

window::wnd* _wnd;
window::dc* _dc;
window::bitmap* _bitmap;

LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	window::cursor cursor_;
	cursor_.load(nullptr, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	window::cls cls;
	cls.set_instance(hInstance);
	cls.set_class_name(L"window");
	cls.set_procedure(procedure);
	cls.set_style(CS_HREDRAW | CS_VREDRAW);
	cls.set_cursor(cursor_);
	cls.register_();

	window::creator creator;
	creator.set_instance(hInstance);
	creator.set_class_name(L"window");
	creator.set_style(WS_OVERLAPPEDWINDOW);
	creator.set_x(CW_USEDEFAULT);
	creator.set_width(CW_USEDEFAULT);

	_wnd = new window::wnd(creator);

	window::dc dc(*_wnd, window::dc::flag::get_dc);
	_dc = new window::dc(dc);
	_bitmap = new window::bitmap(dc);
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
		window::pen pen(PS_SOLID, 2, RGB(255, 0, 0));
		_dc->select_object(pen);
		_dc->move_to(0, 0);
		_dc->line_to(100, 100);
		//_dc->select_object(*_bitmap);

		window::dc dc(*_wnd, window::dc::flag::begin_paint);
		dc.bit_blt(0, 0, 100, 100, *_dc, 0, 0, SRCCOPY);
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