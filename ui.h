#pragma once
#include <functional>
#include <windows.h>

#include "core.h"
class Windows : public IOutPutTarget
{
public:
	//void RegisterEvent(int keycode, std::function<void> callback);
	void SetColor(int x, int y, TGAColor& color) override;
	void screen_update();
	void OutPut() override;
	int screen_close();
	void screen_dispatch();
	void Init(int x, int y, std::string name) override;
	bool NeedClose() override;
	void BeforeOutPot() override;
	//int screen_exit;
	int screen_keys[512];
	int screen_w, screen_h, screen_exit = 0;
	int screen_mx = 0, screen_my = 0, screen_mb = 0;
	 HWND screen_handle = nullptr;		// 主窗口 HWND
	 HDC screen_dc = nullptr;			// 配套的 HDC
	 HBITMAP screen_hb = nullptr;		// DIB
	 HBITMAP screen_ob = nullptr;		// 老的 BITMAP
	unsigned char* screen_fb = nullptr;		// frame buffer
	long screen_pitch = 0;

};