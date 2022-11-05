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
	Renderer renderer(width,height);

	auto ModelPath = "obj/african_head/african_head.obj";
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<IOutPutTarget> imageOutPutTarget(new TGAOutPutTarget());
	std::shared_ptr<IOutPutTarget> screenOutPutTarget(new Windows());
	std::shared_ptr<Model> model(new Model(ModelPath));
	std::shared_ptr<IShader> shader(new GouraudShader());
	std::shared_ptr<Light> light(new Light());
	light->direction = { 1,1,0 };
	light->direction.normalize();
	camera->position = { 1,1,4 };
	camera->up = { 0,1,0 };
	camera->LookAt({ 0,0,0 });
	camera->SetViewPortMatrix(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	camera->SetProjectionMatrix((camera->position - vec3{ 0, 0, 0 }).norm());
	renderer.SetOutPut(imageOutPutTarget);
	renderer.AddModel(model);
	renderer.SetCamera(camera);
	renderer.AddLight(light);
	renderer.AddShader(shader);
	renderer.DoRender();
	screenOutPutTarget->Init(width, height, "test");
}
