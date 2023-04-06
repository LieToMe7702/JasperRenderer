#pragma once
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

class IShader;

class Light
{
public:
	vec3 position;
	vec3 direction;
};


class Camera
{
public:
	vec3 center;
	vec3 forward;
	vec3 up;
	vec3 position;
	vec3 rotation;
	double m_near;
	double m_far;
	double m_nearHalfX;
	double m_nearHalfY;
	void LookAt(vec3 target);
	void SetViewPortMatrix(int xOffset, int yOffset, int screenWidth, int screenHeight);
	void SetProjectionMatrix();
	void SetRotate();
	mat<4, 4> Rotate;
	mat<4, 4> RotateIt;
	mat<4, 4> ModelView;
	mat<4, 4> Viewport;
	mat<4, 4> Projection;
	mat<4, 4> M;
	int depth = 256;

	void SetParam(double near, double far, double nearHalfX, double nearHalfY);

};

class ZBuffer
{
public:
	ZBuffer(int width, int height);
	double GetDepth(int x, int y);
	void SetDepth(int x, int y,double val);
	void Clear();
private:
	std::vector<double> m_buffer;
	int m_width;
	int m_height;
};

class IOutPutTarget
{
public:
	virtual void SetColor(int x, int y, TGAColor& color) = 0;
	virtual void BeforeOutPot() = 0;
	virtual void OutPut() = 0;
	virtual void Init(int x, int y, std::string name) = 0;
	virtual bool NeedClose() = 0;
};

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice);
	void AddModel(std::shared_ptr<Model> model);
	void AddShader(std::shared_ptr<IShader> shader);
	void AddShadowShader(std::shared_ptr<IShader> shader);
	void AddLight(std::shared_ptr<Light> light);
	void DoRender(bool loop);
	void DoShadow();
	Renderer(int screenX, int screenY);
	std::shared_ptr<Model> m_model;
	std::shared_ptr<IOutPutTarget> m_outPut;
	std::shared_ptr<ZBuffer> m_zbuffer;
	std::shared_ptr<ZBuffer> m_shadowBuffer;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<IShader> m_shader;
	std::shared_ptr<IShader> m_shadowShader;
	std::shared_ptr<Light> m_light;
	int m_screenX;
	int m_screenY;
	void DrawTriangle(vec4 vecs[3]);
	void DrawTriangle(vec4 vecs[3], std::shared_ptr<IShader> shader, std::shared_ptr<ZBuffer> buffer, std::shared_ptr<IOutPutTarget> output);
	void DoDraw();
	void DoRender();
	clock_t tick_time;
	int frame;
};

class TGAOutPutTarget : public IOutPutTarget
{
public:
	void SetColor(int x, int y, TGAColor& color) override;
	void OutPut() override;
	void Init(int x, int y, std::string name) override;
	bool NeedClose() override;
	void BeforeOutPot() override;

private:
	std::shared_ptr<TGAImage> m_image;
	std::string name;
};

class IShader
{
public:
	virtual void vertex(const int faceIndex, const int vertIndex, vec4& position);
	virtual bool fragment(const vec3 barycentric, TGAColor& color) = 0;
	virtual void update();
	void AddModel(std::shared_ptr<Model> model);
	void AddLight(std::shared_ptr<Light> light);
	void AddCamera(std::shared_ptr<Camera> camera);
	void AddRender(std::shared_ptr<Renderer> renderer);

	static TGAColor sample2D(const TGAImage& img, vec2& uvf) {
		return img.get(uvf[0] * img.width(), uvf[1] * img.height());
	}
protected:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Light> m_light;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<Renderer> m_render;
	mat<4, 4> uniform_M;
	mat<4, 4> uniform_MIT;
	mat<4, 4> uniform_MShadow;
	vec3 world[3];
	mat<2, 3> varying_uv;  // triangle uv coordinates, written by the vertex shader, read by the fragment shader

};

