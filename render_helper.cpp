#include "render_helper.h"
void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color)
{
	auto steep = false;
	if(std::abs(x0-x1) < std::abs(y0-y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if(x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for(auto x = x0; x < x1;x++)
	{
		auto t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1.0 - t) + y1 * t;
		steep ? image.set(y, x, color) : image.set(x, y, color);
	}
}

void line2(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
	auto steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	auto dx = x1 - x0;
	auto dy = y1 - y0;
	auto derror2 = std::abs(dy) * 2;
	auto error2 = 0;
	auto dx2 = dx * 2;

	for (auto x = x0,y = y0; x < x1; x++)
	{
		steep ? image.set(y, x, color) : image.set(x, y, color);
		error2 += derror2;
		if(error2 >dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx2;
		}
	}
}
