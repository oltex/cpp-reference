#pragma once
#include <Windows.h>

inline auto message(void) noexcept -> int {
	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
};

class window {
public:
	struct WNDSTRUCTEXW {
		DWORD dwExStyle;
		LPCWSTR lpClassName;
		LPCWSTR lpWindowName;
		DWORD dwStyle;
		int x;
		int y;
		int nWidth;
		int nHeight;
		HWND hWndParent;
		HMENU hMenu;
		HINSTANCE hInstance;
		LPVOID lpParam;
	};
public:
	explicit window(void) noexcept = default;
	virtual ~window(void) noexcept;
protected:
	static auto initialize_class(HINSTANCE const instance) noexcept -> WNDCLASSEXW;
	static auto register_class(WNDCLASSEXW const wcex) noexcept -> ATOM;
	static auto initialize_window(HINSTANCE const instance) noexcept -> WNDSTRUCTEXW;
protected:
	void create(WNDSTRUCTEXW const wsex) noexcept;
public:
	inline auto show(int const nCmdShow) const noexcept -> BOOL {
		return ShowWindow(_wnd, nCmdShow);
	}
	inline auto move(int const x, int const y, int const width, int const height) const noexcept -> BOOL {
		return MoveWindow(_wnd, x, y, width, height, false);
	}
	inline auto update(void) const noexcept -> BOOL {
		return UpdateWindow(_wnd);
	}
	inline auto close(void) const noexcept {
		CloseWindow(_wnd);
	}
	inline auto destroy(void) const noexcept -> BOOL {
		return DestroyWindow(_wnd);
	}
	//static LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;
protected:
	HWND _wnd = nullptr;
};

class AAA : public window {
	AAA(HINSTANCE const instance) {
		WNDCLASSEXW wcex = initialize_class(instance);
		register_class(wcex);
		WNDSTRUCTEXW wsex = initialize_window(instance);

		create(wsex);
	}
	static LRESULT CALLBACK procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
		//wnd??
	}
};