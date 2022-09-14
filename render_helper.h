#pragma once
#include "tgaimage.h"
#include "geometry.h"
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void line2(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color);
void triangle_hollow(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color);