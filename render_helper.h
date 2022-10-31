#pragma once
#include "tgaimage.h"
#include "geometry.h"
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void line2(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color);
void triangle(vec3 t0, vec3 t1, vec3 t2, TGAImage& image, const TGAColor& color, std::vector<double>& z_buffer);
void triangle(const std::vector<vec3>& points, const std::vector<vec2>& uvs, TGAImage& image, const TGAImage& diff, std::vector<double>& z_buffer,float intensity);
void triangle_hollow(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, const TGAColor& color);
vec3 barycentric(vec3 points[3], const vec2& p);
vec3 barycentric(vec2 points[3], const vec2& p);