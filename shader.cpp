#include "core.h"
#include "shader.h"

void NormalShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	position = embed<4>(res);
}

bool NormalShader::fragment(const vec3 barycentric, TGAColor& color)
{
	return false;
}
