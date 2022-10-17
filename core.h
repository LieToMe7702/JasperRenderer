#pragma once
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

class Camera
{
public:
	vec3 position;
	vec3 forward;
	vec3 up;
	void LookAt(vec3 target);
	void SetViewPortMatrix(int xOffset, int yOffset, int screenWidth, int screenHeight);
	void SetProjectionMatrix(float coeff);

};

class ZBuffer
{
public:
	ZBuffer(int width, int height);
};

class IOutPutTarget
{
public:
	virtual void SetColor(int x, int y, TGAColor color) = 0;
	virtual void OutPut() = 0;

};

class Renderer
{
public:
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetZBuffer(std::shared_ptr<ZBuffer> zBuffer);
	void SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice);
	void AddModel(std::shared_ptr<Model> model);
	void DoRender();
private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<IOutPutTarget> m_outPut;
	std::shared_ptr<ZBuffer> m_zbuffer;
	std::shared_ptr<Camera> m_camera;
};

class TGAOutPutTarget : public IOutPutTarget
{
public:
	void SetColor(int x, int y, TGAColor color) override;
	void OutPut() override;
	TGAOutPutTarget(int x, int y, std::string name);
private:
	std::shared_ptr<TGAImage> m_image;
	std::string name;
};

