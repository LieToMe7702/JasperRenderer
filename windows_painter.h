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
	static int screen_keys[512]; // ��ǰ���̰���״̬
	unsigned char* screen_fb; // frame buffer
private:
	int screen_init(int w, int h, const TCHAR* title); // ��Ļ��ʼ��
	int screen_close(void); // �ر���Ļ
	void screen_dispatch(void); // ������Ϣ
	void screen_update(void); // ��ʾ FrameBuffer

	// win32 event handler

	HWND screen_handle; // ������ HWND
	HDC screen_dc; // ���׵� HDC
	HBITMAP screen_hb; // DIB
	HBITMAP screen_ob; // �ϵ� BITMAP
	long screen_pitch = 0;
};
