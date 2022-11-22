#pragma once
#include "core.h"

class GouraudShader : public IShader
{
public:
	void vertex(const int faceIndex, const int vertIndex, vec4& position) override;
	bool fragment(const vec3 barycentric, TGAColor& color) override;
private:
	vec3 varying_intensity;
	mat<3, 3> varying_nrm; // normal per vertex to be interpolated by FS
	mat<3, 3> view_tri;    // triangle in view coordinates

};

class PhongShader : public IShader
{
public:
	bool fragment(const vec3 barycentric, TGAColor& color) override;
private:
	mat<3, 3> varying_nrm; // normal per vertex to be interpolated by FS
	mat<3, 3> view_tri;    // triangle in view coordinates
};

