#pragma once
#include "core.h"

class GouraudShader : public IShader
{
public:
	void vertex(const int faceIndex, const int vertIndex, vec4& position) override;
	bool fragment(const vec3 barycentric, TGAColor& color) override;
private:
	vec3 world[3];
	vec3 varying_intensity;
	mat<2, 3> varying_uv;  // triangle uv coordinates, written by the vertex shader, read by the fragment shader
	mat<3, 3> varying_nrm; // normal per vertex to be interpolated by FS
	mat<3, 3> view_tri;    // triangle in view coordinates

};

class PhongShader : public IShader
{
public:
	void vertex(const int faceIndex, const int vertIndex, vec4& position) override;
	bool fragment(const vec3 barycentric, TGAColor& color) override;
private:
	vec3 world[3];
	vec3 varying_intensity;
	mat<2, 3> varying_uv;  // triangle uv coordinates, written by the vertex shader, read by the fragment shader
	mat<3, 3> varying_nrm; // normal per vertex to be interpolated by FS
	mat<3, 3> view_tri;    // triangle in view coordinates
};

