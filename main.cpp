#include "model.h"
#include "tgaimage.h"
#include "render_helper.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);


int main(void)
{
	auto width = 1024;
	auto height = 768;
	TGAImage image(width, height, TGAImage::RGB);
	Model model("obj/african_head/african_head.obj");
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
	image.write_tga_file("output.tga");
	return 0;
}
