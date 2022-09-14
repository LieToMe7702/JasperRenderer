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

void line(vec2 v1, vec2 v2, TGAImage& image, const TGAColor& color)
{
	line2(v1.x, v1.y, v2.x, v2.y, image, color);
}

void triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color)
{
	int y0 = t0.y;
	int y1 = t1.y;
	int y2 = t2.y;
	if (y0 > y1) std::swap(y0, y1);
	if (y0 > y2) std::swap(y0, y2);
	if (y1 > y2) std::swap(y1, y2);

	if(y0 == y1 == y2)
	{
		std::cerr << "Error: the triangle height is error" << std::endl;
		return;
	}
	auto total_height = y2 - y0;
	auto segmeng_height = y1 - y0;

	for(auto y = y0; y < y1;y++)
	{
		auto a = total_height <= 0 ? 0: (y - t0.y) / total_height;
		auto b = segmeng_height <= 0 ? 0 :(y - t0.y) / segmeng_height;
		auto A = t0 + (t2 - t0) * a;
		auto B = t0 + (t1 - t0) * b;
		line2(A.x, A.y, B.x, B.y, image,color);
	}
	segmeng_height = y2 - y1;

	for (auto y = y1; y < y2; y++)
	{
		auto a = total_height <= 0 ? 0 : (y - t0.y) / total_height;
		auto b = segmeng_height <= 0 ? 0 : (y - t1.y) / segmeng_height;
		auto A = t0 + (t2 - t0) * a;
		auto B = t1 + (t2 - t1) * b;
		line2(A.x, A.y, B.x, B.y, image, color);
	}

}

void triangle_hollow(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color)
{
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	line(t0, t1, image, color);
	line(t1, t2, image, color);
	line(t2, t0, image, color);
}
