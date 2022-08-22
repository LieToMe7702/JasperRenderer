#pragma once
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif
#include <windows.h>
#include <tchar.h>
#include <functional>

class WindowsPainter
{
public:
	bool screen_init(int w, int h);

	void update(const std::function<void()>& func);
	static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);
	static int screen_w, screen_h, screen_exit;
	static int screen_mx, screen_my, screen_mb;
	static int screen_keys[512]; // 当前键盘按下状态
	unsigned char* screen_fb; // frame buffer
private:
	int screen_init(int w, int h, const TCHAR* title); // 屏幕初始化
	int screen_close(void); // 关闭屏幕
	void screen_dispatch(void); // 处理消息
	void screen_update(void); // 显示 FrameBuffer

	// win32 event handler

	HWND screen_handle; // 主窗口 HWND
	HDC screen_dc; // 配套的 HDC
	HBITMAP screen_hb; // DIB
	HBITMAP screen_ob; // 老的 BITMAP
	long screen_pitch = 0;
};
