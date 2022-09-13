#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);


int main(void)
{
	auto width = 800;
	auto height = 800;
	TGAImage image(width, height, TGAImage::RGB);
	std::string path = "obj/african_head/african_head.obj";
	std::string name = path;
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
			line(x0, y0, x1, y1, image, white);
		}
	}
	//line(702, 666, 10, 20, image,white);
	//line2(555, 375, 15, 15, image, red);
	//line(555, 375, 15, 15, image, white);
	//image.flip_vertically();
	image.write_tga_file(name);
	return 0;
}
