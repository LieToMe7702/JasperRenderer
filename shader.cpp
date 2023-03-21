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

void NormalTangentMappingShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	auto n = m_model->normal(faceIndex, vertIndex);
	varying_nrm.set_col(vertIndex, proj<3>(m_camera->RotateIt *embed<4>(n, 0)));
}

bool NormalTangentMappingShader::fragment(const vec3 barycentric, TGAColor& color)
{
	vec3 bn = (varying_nrm * barycentric).normalize(); // per-vertex normal interpolation
	vec2 uv = varying_uv * barycentric;
	auto diffColor = m_model->diffuse(uv.x, uv.y);

	mat<3, 3> AI = mat<3, 3>{{world[1] - world[0], world[2] - world[0], bn}}.invert();
	vec3 i = AI * vec3(varying_uv[0][1] - varying_uv[0][0], varying_uv[0][2] - varying_uv[0][0], 0);
	vec3 j = AI * vec3(varying_uv[1][1] - varying_uv[1][0], varying_uv[1][2] - varying_uv[1][0], 0);
	mat<3, 3> B = mat<3, 3>{{i.normalize(), j.normalize(), bn}}.transpose();

	vec3 n = (B * m_model->normal_tangent(uv)).normalize();
	// transform the normal from the texture to the tangent space
	auto l = m_light->direction.normalize();

	auto intensity = std::max(static_cast<double>(0), n * l);
	diffColor = diffColor * intensity;
	color = diffColor;
	return false;
}

