#include "window.h"

window::~window(void) noexcept {
	destroy();
}

auto window::initialize_class(HINSTANCE const instance) noexcept -> WNDCLASSEXW {
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProcW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = nullptr;
	wcex.hIconSm = nullptr;
	return wcex;
}

auto window::register_class(WNDCLASSEXW const wcex) noexcept -> ATOM {
	return RegisterClassExW(&wcex);
}

auto window::initialize_window(HINSTANCE const instance) noexcept -> WNDSTRUCTEXW {
	WNDSTRUCTEXW wsex;
	wsex.dwExStyle = 0L;
	wsex.lpClassName = nullptr;
	wsex.lpWindowName = nullptr;
	wsex.dwStyle = WS_OVERLAPPEDWINDOW;
	wsex.x = 0;
	wsex.y = 0;
	wsex.nWidth = CW_USEDEFAULT;
	wsex.nHeight = 0;
	wsex.hWndParent = nullptr;
	wsex.hMenu = nullptr;
	wsex.hInstance = instance;
	wsex.lpParam = nullptr;
	return wsex;
}

void window::create(WNDSTRUCTEXW const wsex) noexcept {
	_wnd = CreateWindowExW(wsex.dwExStyle, wsex.lpClassName, wsex.lpWindowName, wsex.dwStyle,
		wsex.x, wsex.y, wsex.nWidth, wsex.nHeight, wsex.hWndParent, wsex.hMenu, wsex.hInstance, wsex.lpParam);
}

//LRESULT window::procedure(HWND const wnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
	//switch (message)
	//{
	//case WM_COMMAND:
	//{
	//    int wmId = LOWORD(wparam);
	//    // 메뉴 선택을 구문 분석합니다:
	//    switch (wmId)
	//    {
	//    case IDM_ABOUT:
	//        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	//        break;
	//    case IDM_EXIT:
	//        DestroyWindow(hWnd);
	//        break;
	//    default:
	//        return DefWindowProc(hWnd, message, wparam, lparam);
	//    }
	//}
	//break;
	//case WM_PAINT:
	//{
	//    PAINTSTRUCT ps;
	//    HDC hdc = BeginPaint(hWnd, &ps);
	//    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	//    EndPaint(hWnd, &ps);
	//}
	//break;
	//case WM_DESTROY:
	//    PostQuitMessage(0);
	//    break;
	//default:
	//    return DefWindowProc(hWnd, message, wparam, lparam);
	//}
//	return 0;
//}
