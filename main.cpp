#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
#include "core.h"
#include "shader.h"
#include "ui.h"

void Move(const std::shared_ptr<Camera>& camera, float x, float y, float z)
{
	camera->position = camera->position + vec3{ x, y, z };
}

void Loop(double &x, double i)
{
	if(x > i)
	{
		x -= i;
	}else if(x < -i)
	{
		x += i;
	}
}

void Rotate(const std::shared_ptr<Camera>& camera, float x, float y, float z)
{
	camera->rotation = camera->rotation + vec3(x, y, z);
	Loop(camera->rotation.x, 360);
	Loop(camera->rotation.y, 360);
	Loop(camera->rotation.z, 360);
}

void RegisterKeyEvent(std::shared_ptr<Camera> camera, std::shared_ptr<Windows> outPutTarget)
{
	outPutTarget->RegisterEvent('W', [&] { Move(camera, 0, 0.1, 0); });
	outPutTarget->RegisterEvent('S', [&] { Move(camera, 0, -0.1, 0); });
	outPutTarget->RegisterEvent('A', [&] { Move(camera, 0.1, 0, 0); });
	outPutTarget->RegisterEvent('D', [&] { Move(camera, -0.1, 0, 0); });
	outPutTarget->RegisterEvent(VK_LEFT, [&] { Rotate(camera, 0, -0.1, 0); });
	outPutTarget->RegisterEvent(VK_RIGHT, [&] { Rotate(camera, 0, 0.1, 0); });
}

int main(void)
{
	auto width = 800;
	auto height = 800;
	Renderer renderer(width, height);

	auto ModelPath = "obj/african_head/african_head.obj";
	std::shared_ptr<Camera> camera(new Camera());
	//std::shared_ptr<IOutPutTarget> outPutTarget(new TGAOutPutTarget());
	std::shared_ptr<Windows> outPutTarget(new Windows());
	RegisterKeyEvent(camera, outPutTarget);
	std::shared_ptr<Model> model(new Model(ModelPath));
	std::shared_ptr<IShader> shader(new GouraudShader());
	//std::shared_ptr<IShader> shader(new PhongShader());
	std::shared_ptr<Light> light(new Light());
	light->direction = { 0,0,1 };
	light->direction.normalize();
	camera->position = { 0,0,1 };
	camera->up = { 0,1,0 };

	renderer.SetOutPut(outPutTarget);
	renderer.AddModel(model);
	renderer.SetCamera(camera);
	renderer.AddLight(light);
	renderer.AddShader(shader);
	renderer.DoRender(true);
	//screenOutPutTarget->Init(width, height, "test");
}
