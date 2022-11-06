#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
#include "core.h"
#include "shader.h"
#include "ui.h"

int main(void)
{
	auto width = 800;
	auto height = 800;
	Renderer renderer(width, height);

	auto ModelPath = "obj/african_head/african_head.obj";
	std::shared_ptr<Camera> camera(new Camera());
	//std::shared_ptr<IOutPutTarget> outPutTarget(new TGAOutPutTarget());
	std::shared_ptr<Windows> outPutTarget(new Windows());
	auto degree = 1;
	outPutTarget->RegisterEvent(VK_UP, [&]()
	{
		camera->position = camera->position + vec3{0, 0, 1};
	});
	outPutTarget->RegisterEvent(VK_DOWN, [&]()
	{
		camera->position = camera->position + vec3{0, 0, -1};
	});
	outPutTarget->RegisterEvent(VK_LEFT, [&]()
		{
			camera->position = camera->position + vec3{ -0.1, 0, 0 };
		});
	outPutTarget->RegisterEvent(VK_RIGHT, [&]()
		{
			camera->position = camera->position + vec3{ 0.1, -0, 0 };
		});
	std::shared_ptr<Model> model(new Model(ModelPath));
	std::shared_ptr<IShader> shader(new GouraudShader());
	std::shared_ptr<Light> light(new Light());
	light->direction = { 1,1,0 };
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
