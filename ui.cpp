#include "ui.h"

#include <tchar.h>
static Windows *windows;
static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: windows->screen_exit = 1; break;
	case WM_KEYDOWN: windows->screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: windows->screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Windows::RegisterEvent(int keycode, std::function<void()> callback)
{
	if(callbackMap.count(keycode) == 0)
	{
		callbackMap[keycode] = {};
	}
	auto& list = callbackMap[keycode];
	list.emplace_back(std::move(callback));
}

void Windows::SetColor(int x, int y, TGAColor& color)
{
	//y = screen_h - y - 1;
	auto offset = screen_w * 4 * y + 4 * x;
	screen_fb[offset] = color.bgra[0];
	screen_fb[offset + 1] = color.bgra[1];
	screen_fb[offset + 2] = color.bgra[2];
	screen_fb[offset + 3] = color.bgra[3];
}

void Windows::screen_update()
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}

void Windows::OutPut()
{
	screen_update();
}

int Windows::screen_close()
{
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

void Windows::screen_dispatch()
{
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void Windows::Init(int w, int h, std::string name)
{
	windows = this;
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		static_cast<unsigned long>(w * h * 4), 0, 0, 0, 0 } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return ;

	size_t origsize = name.length() + 1;
	 const size_t newsize = 100;
	 size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (name.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, name.c_str(), _TRUNCATE);
	screen_handle = CreateWindow(_T("SCREEN3.1415926"), wcstring,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return ;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return ;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0,  sizeof(unsigned char) * w * h * 4);

	return ;
}

bool Windows::NeedClose()
{
	return !(screen_exit == 0 && screen_keys[VK_ESCAPE] == 0);
}

void Windows::handle_input()
{
	for (int i = 0; i < 512; i++)
	{
		if (screen_keys[i] != 0 && callbackMap.count(i) != 0)
		{
			auto& callbacks = callbackMap[i];
			clear_old_pixel();
			for (auto& it : callbacks)
			{
				it();
			}
		}
	}
}

void Windows::clear_old_pixel()
{
	memset(screen_fb, 0, sizeof(unsigned char) * screen_w * screen_h * 4);
}

void Windows::BeforeOutPot()
{
	screen_dispatch();
	clear_old_pixel();
	handle_input();
}
