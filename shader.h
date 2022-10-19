#pragma once
#include "core.h"

class NormalShader : public IShader
{
public:
	void vertex(const int faceIndex, const int vertIndex, vec4& position) override;
	bool fragment(const vec3 barycentric, TGAColor& color) override;
private:
	vec2 uvs[3];
	vec3 world[3];
};

