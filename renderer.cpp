#include "renderer.h"

void Renderer::Init(int w, int h, unsigned char* frameBuffer)
{
	this->w = w;
	this->h = h;
	this->frameBuffer = frameBuffer;
}

void Renderer::Clear()
{
}

void Renderer::SetCamera()
{
}

void Renderer::Draw()
{
}

void Renderer::Update()
{
	Clear();
	SetCamera();
	Draw();
}
