#include "core.h"
#include "shader.h"

void GouraudShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	vec3 n = proj<3>(m_camera->RotateIt * embed<4>(m_model->normal(faceIndex, vertIndex), 0)).normalize();
	vec3 l = proj<3>(embed<4>(m_light->direction)).normalize();
	auto intensity = std::max(static_cast<double>(0), n * l);
	varying_intensity[vertIndex] = intensity;

}

bool GouraudShader::fragment(const vec3 barycentric, TGAColor& color)
{
	auto intensity = varying_intensity * barycentric;
	vec2 uv = varying_uv * barycentric;
	auto  diffColor = m_model->diffuse(uv.x, uv.y);
	diffColor = diffColor * intensity;
	color = diffColor;

	return false;
}
void PhongShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	auto n = m_model->normal(faceIndex, vertIndex);
	varying_nrm.set_col(vertIndex, proj<3>(m_camera->RotateIt *embed<4>(n, 0)));
}

bool PhongShader::fragment(const vec3 barycentric, TGAColor& color)
{
	vec2 uv = varying_uv * barycentric;
	auto  diffColor = m_model->diffuse(uv.x, uv.y);
	auto l = m_light->direction.normalize();
	auto n = varying_nrm * barycentric;
	auto intensity = std::max(static_cast<double>(0), n * l);
	diffColor = diffColor * intensity;
	color = diffColor;

	return false;
}

bool NormalMappingShader::fragment(const vec3 barycentric, TGAColor& color)
{
	vec2 uv = varying_uv * barycentric;
	auto  diffColor = m_model->diffuse(uv.x, uv.y);
	auto l = m_light->direction.normalize();
	auto n = proj<3>(m_camera->RotateIt * embed<4>(m_model->normal(uv),0)).normalize();
	auto intensity = std::max(static_cast<double>(0), n * l);
	diffColor = diffColor * intensity;
	color = diffColor;

	return false;
}

