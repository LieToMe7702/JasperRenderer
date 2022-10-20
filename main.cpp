#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
#include "core.h"
#include "shader.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

vec3 light_dir(0, 0, 1);

Model GetModel(TGAImage& image, std::string& name)
{
	auto width = image.width();
	auto height = image.height();
	std::string path = "obj/african_head/african_head.obj";
	name = path;
	auto index = path.find_last_of("/");
	if (index > 0)
	{
		name = path.substr(index + 1);
	}
	name += std::to_string(width) + "x" + std::to_string(height) + ".tga";
	Model model(path);
	return model;
}


void DrawColorModel(TGAImage& image, std::string& name)
{
	auto model = GetModel(image, name);
	auto width = image.width();
	auto height = image.height();
	std::vector<vec2> screen_coords(3);
	for (int i = 0; i < model.nfaces(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto world_coords = model.vert(i, j);
			screen_coords[j] = vec2((world_coords.x + 1) * width / 2, (world_coords.y + 1) * height / 2);
		}
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand() % 255, rand() % 255, rand() % 255));
	}
}

void DrawLightModel(TGAImage& image, std::string& name)
{
	auto model = GetModel(image, name);
	auto width = image.width();
	auto height = image.height();
	std::vector<vec3> screen_coords(3);
	std::vector<vec3> world_coords(3);
	std::vector<vec2> uv_coords(3);
	std::vector<double> z_buffer(width * height, std::numeric_limits<double>::min());
	auto depth = 256;
	for (int i = 0; i < model.nfaces(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto world_coord = model.vert(i, j);
			screen_coords[j] = vec3((int)((world_coord.x + 1) * width / 2),
			                        (int)((world_coord.y + 1) * height / 2), (int)((world_coord.z + 1) * depth / 2));
			world_coords[j] = world_coord;
			uv_coords[j] = model.uv(i, j);
		}
		auto normal = cross((world_coords[1] - world_coords[0]), (world_coords[2] - world_coords[0]));
		normal.normalize();
		auto intensity = normal * light_dir;
		if(intensity > 0)
		{
			triangle(screen_coords, uv_coords, image, model.diffuse(), z_buffer, intensity);
		}
	}
}


void DrawModel(TGAImage& image, std::string& name)
{
	auto model = GetModel(image, name);
	auto width = image.width();
	auto height = image.height();
	for(int i = 0; i < model.nfaces();i++)
	{
		for(int j = 0 ; j < 3; j++)
		{
			auto v0 = model.vert(i, j);
			auto v1 = model.vert(i, (j + 1)%3);
			int x0 = (v0.x + 1.0) * width / 2;
			int y0 = (v0.y + 1.0) * height / 2;
			int x1 = (v1.x + 1.0) * width / 2;
			int y1 = (v1.y + 1.0) * height / 2;
			line2(x0, y0, x1, y1, image, white);
		}
	}
}

void DrawTriangle(TGAImage& image, const TGAColor& color)
{
	vec2 v1(100, 200);
	vec2 v2(200, 300);
	vec2 v3(400, 600);
	triangle(v1, v2, v3, image, color);
}

/*
int main(void)
{
	auto width = 800;
	auto height = 800;
	TGAImage image(width, height, TGAImage::RGB);
	std::string name = "output.tga";
	//DrawModel(width, height, image, name);
	DrawLightModel(image, name);
	//DrawTriangle(image, red);
	//line(702, 666, 10, 20, image,white);
	//line2(555, 375, 15, 15, image, red);
	//line(555, 375, 15, 15, image, white);
	//image.flip_vertically();
	image.write_tga_file(name);
	return 0;
}*/

int main(void)
{
	auto width = 800;
	auto height = 800;
	Renderer renderer(width,height);

	auto ModelPath = "obj/african_head/african_head.obj";
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<IOutPutTarget> imageOutPutTarget(new TGAOutPutTarget());
	std::shared_ptr<Model> model(new Model(ModelPath));
	std::shared_ptr<IShader> shader(new GouraudShader());
	std::shared_ptr<Light> light(new Light());
	light->direction = { 0,0,1 };
	renderer.SetOutPut(imageOutPutTarget);
	renderer.AddModel(model);
	renderer.SetCamera(camera);
	renderer.AddLight(light);
	renderer.AddShader(shader);
	renderer.DoRender();
}
