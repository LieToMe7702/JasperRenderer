#include "core.h"
#include "shader.h"

void GouraudShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	varying_intensity[vertIndex] = std::max(static_cast<double>(0), m_model->normal(faceIndex, vertIndex) * m_light->direction);

}

bool GouraudShader::fragment(const vec3 barycentric, TGAColor& color)
{
	auto& diff = m_model->diffuse();
	auto intensity = varying_intensity * barycentric;
	vec2 uv = varying_uv * barycentric;
	auto  diffColor = m_model->diffuse(uv.x, uv.y);
	diffColor = diffColor * intensity;
	color = diffColor;

	return false;
}


bool PhongShader::fragment(const vec3 barycentric, TGAColor& color)
{
	vec2 uv = varying_uv * barycentric;
	auto  diffColor = m_model->diffuse(uv.x, uv.y);
	vec3 n = proj<3>(uniform_MIT * embed<4>(m_model->normal(uv))).normalize();
	vec3 l = m_light->direction.normalize();
	auto intensity = std::max(static_cast<double>(0), n * l);
	diffColor = diffColor * intensity;
	color = diffColor;

	return false;
}

