#pragma once
#include "core.h"

class NormalShader : public IShader
{
public:
	void vertex(const int faceIndex, const int vertIndex, vec4& position) override;
	bool fragment(const vec3 barycentric, TGAColor& color) override;
};

