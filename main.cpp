#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);


void DrawModel(int width, int height, TGAImage& image, std::string& name)
{
	std::string path = "obj/african_head/african_head.obj";
	name = path;
	auto index = path.find_last_of("/");
	if(index > 0)
	{
		name = path.substr(index + 1);
	}
	name += std::to_string(width) + "x" + std::to_string(height) + ".tga";
	Model model(path);
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

int main(void)
{
	auto width = 1000;
	auto height = 1000;
	TGAImage image(width, height, TGAImage::RGB);
	std::string name = "output.tga";
	//DrawModel(width, height, image, name);
	DrawTriangle(image, red);
	//line(702, 666, 10, 20, image,white);
	//line2(555, 375, 15, 15, image, red);
	//line(555, 375, 15, 15, image, white);
	//image.flip_vertically();
	image.write_tga_file(name);
	return 0;
}
