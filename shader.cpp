#include "core.h"
#include "shader.h"

void GouraudShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	world[vertIndex] = res;
	varying_intensity[vertIndex] = std::max(static_cast<double>(0), m_model->normal(faceIndex, vertIndex) * m_light->direction);
	position = embed<4>(res);
	position = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate * position;

	varying_uv.set_col(vertIndex, m_model->uv(faceIndex, vertIndex));
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


void PhongShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	world[vertIndex] = res;
	varying_intensity[vertIndex] = std::max(static_cast<double>(0), m_model->normal(faceIndex, vertIndex) * m_light->direction);
	position = embed<4>(res);
	position = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate * position;

	varying_uv.set_col(vertIndex, m_model->uv(faceIndex, vertIndex));
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

