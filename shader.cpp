#include "core.h"
#include "shader.h"

void GouraudShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	uvs[vertIndex] = m_model->uv(faceIndex, vertIndex);
	world[vertIndex] = res;
	varying_intensity[vertIndex] = std::max(static_cast<double>(0), m_model->normal(faceIndex, vertIndex) * m_light->direction);
	position = embed<4>(res);
	position = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * position;
}

bool GouraudShader::fragment(const vec3 barycentric, TGAColor& color)
{
	auto& diff = m_model->diffuse();
	auto newUV = uvs[0] * barycentric[0] + uvs[1] * barycentric[1] + uvs[2] * barycentric[2];
	auto diffU = diff.width() * newUV.x;
	auto diffY = diff.height() * newUV.y;
	auto diffColor = diff.get(diffU, diffY);
	auto intensity = varying_intensity * barycentric;
	diffColor[0] *= intensity;
	diffColor[1] *= intensity;
	diffColor[2] *= intensity;
	color = diffColor;
	return false;
}
