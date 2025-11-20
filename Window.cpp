


#include "Window.h"
#include "resource.h"

LPCWSTR Window::WindowClass::wndClassName = L"placeholder";
Window::WindowClass Window::WindowClass::wndclass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.lpfnWndProc = HandleMsgSetup;
	//wc.hIcon = static_cast<HICON>(LoadImageW(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	//wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	wc.hCursor = static_cast<HCURSOR>(LoadCursorW(hInstance, MAKEINTRESOURCE(IDC_CURSOR1)));
	//wc.hCursor = LoadCursorW(NULL, IDC_CROSS);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();

	RegisterClassEx(&wc);
}

HINSTANCE Window::WindowClass::GetInstance() {
	return wndclass.hInstance;
}

LPCWSTR Window::WindowClass::GetName() {
	return wndClassName;
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

Window::Window(int width, int height, LPCWSTR ClassName) : width(width) , height(height)
{

	RECT dimensions;
	dimensions.top = 100;
	dimensions.left = 100;
	dimensions.right = dimensions.left + width;
	dimensions.bottom = dimensions.top + height;

	hwnd = CreateWindowExW(
		0,
		WindowClass::GetName(),
		ClassName,
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE,

		CW_USEDEFAULT,CW_USEDEFAULT,dimensions.right,dimensions.bottom,

		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this
	);

	ShowWindow(hwnd,SW_SHOWDEFAULT);

	pgfx = std::make_unique<Graphics>(hwnd);

}

Window::~Window() {
	DestroyWindow(hwnd);
}

Graphics& Window::gfx()
{
	if (!pgfx) {
		//oh no
		throw "GRAPHICS ARE FRIED";
	}
	return *pgfx;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return {};

}

void Window::SetWindowTitle(LPCSTR name)
{
	//debug remove later

	SetWindowTextA(hwnd, name);

}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {

	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wparam, lparam);
	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {

	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hWnd, msg, wparam, lparam);
}

LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {

	switch (msg) {
	//KEYBOARD
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: {
		kbd.KeyDown(wparam);
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		kbd.KeyUp(wparam);
		break;
	}
	case WM_CHAR: {
		kbd.Char(wparam);
		break;
	}
	case WM_KILLFOCUS: {
		kbd.ClearKeyboard();
		break;
	}
	//MOUSE
	case WM_MOUSEMOVE: {
		const POINTS pt = MAKEPOINTS(lparam);

		if (pt.x > 0 and pt.x < width and pt.y > 0 and pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.MouseIsInWindow()) {
				SetCapture(hWnd);
				mouse.MouseEnter();
			}

		}
		else if (wparam and (MK_LBUTTON or MK_RBUTTON)) {
			mouse.OnMouseMove(pt.x, pt.y);
		}
		else {
			ReleaseCapture();
			mouse.MouseExit();
		}
		break;
	}
	case WM_LBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.LeftDown(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.RightDown(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.LeftUp(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.RightUp(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL: {
		const POINTS pt = MAKEPOINTS(lparam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wparam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//QUIT
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}