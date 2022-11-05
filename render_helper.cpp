#include "render_helper.h"

vec3 barycentric(vec2 points[3], const vec2& p)
{
	vec3 vec_x(points[1][0] - points[0][0], points[2][0] - points[0][0], points[0][0] - p[0]);
	vec3 vec_y(points[1][1] - points[0][1], points[2][1] - points[0][1], points[0][1] - p[1]);
	vec3 cross_vec = cross(vec_x, vec_y);
	if (std::abs(cross_vec.z) < 1e-3) return { -1, 1, 1 };
	vec3 res{ 1.0f - (cross_vec.x + cross_vec.y) / cross_vec.z,cross_vec.x / cross_vec.z,cross_vec.y / cross_vec.z };
	return res;
}



