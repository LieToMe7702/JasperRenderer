#include "tgaimage.h"
#include "render_helper.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);


int main(void)
{
	TGAImage image(1080, 720, TGAImage::RGB);
	//line(702, 666, 10, 20, image,white);
	line2(555, 375, 15, 15, image, red);
	//line(555, 375, 15, 15, image, white);
	//image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
