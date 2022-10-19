#pragma once
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

class IShader;

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
	virtual void Init(int x, int y, std::string name) = 0;
};

class Renderer
{
public:
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice);
	void AddModel(std::shared_ptr<Model> model);
	void AddShader(std::shared_ptr<IShader> shader);
	void DoRender();
	Renderer(int screenX, int screenY);
private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<IOutPutTarget> m_outPut;
	std::shared_ptr<ZBuffer> m_zbuffer;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<IShader> m_shader;
	int m_screeX;
	int m_screenY;
	void DrawTriangle(vec4 vecs[3]);
};

class TGAOutPutTarget : public IOutPutTarget
{
public:
	void SetColor(int x, int y, TGAColor color) override;
	void OutPut() override;
	void Init(int x, int y, std::string name) override;
private:
	std::shared_ptr<TGAImage> m_image;
	std::string name;
};

class IShader
{
public:
	virtual void vertex(const int faceIndex, const int vertIndex, vec4& position) = 0;
	virtual bool fragment(const vec3 barycentric, TGAColor& color) = 0;
	void AddModel(std::shared_ptr<Model> model);
protected:
	std::shared_ptr<Model> m_model;
};

