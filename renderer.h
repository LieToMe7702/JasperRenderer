#pragma once
class Renderer
{
public:
	void Init(int w, int h, unsigned char* frameBuffer);
	void Clear();
	void SetCamera();
	void Draw();
	void Update();
	int w;
	unsigned char* frameBuffer;
	int h;
};
