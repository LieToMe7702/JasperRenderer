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
	diffColor = diffColor * intensity ;
	color = diffColor;

	return false;
}

void NormalTangentMappingShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	auto n = m_model->normal(faceIndex, vertIndex);
	varying_nrm.set_col(vertIndex, n.normalize());
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

void NormalTangentMappingWithPhongReflectionShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	auto n = m_model->normal(faceIndex, vertIndex);
	varying_nrm.set_col(vertIndex, n.normalize());
}

bool NormalTangentMappingWithPhongReflectionShader::fragment(const vec3 barycentric, TGAColor& color)
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
	auto r = (2 * n * (n * l) - l).normalize();
	auto specColor = m_model->spec(uv.x,uv.y);
	auto spec = pow(std::max<double>(r.z, 0),specColor[0]);
	auto intensity = std::max(static_cast<double>(0), n * l);
	for(int i = 0; i < 3; i++)
	{
		diffColor[i] = std::min<float>(10 + diffColor[i]  * (0.8 * intensity + 0.1 * spec), 255);
	}
	color = diffColor;
	return false;
}

void NormalTangentMappingWithPhongReflectionAndShadowMapShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
	auto n = m_model->normal(faceIndex, vertIndex);
	varying_nrm.set_col(vertIndex, n.normalize());
}

bool NormalTangentMappingWithPhongReflectionAndShadowMapShader::fragment(const vec3 barycentric, TGAColor& color)
{
	auto shadowPoint = uniform_MShadow * embed<4>(varying_tri * barycentric);
	shadowPoint = shadowPoint / shadowPoint[3];
	int x = shadowPoint[0];
	int y = shadowPoint[1];
	auto shadowVal = 0.3;
	if(x >= 0 && x< m_render->m_screenX && y>= 0 && y < m_render->m_screenY)
	{
		shadowVal = 0.3 + 0.7 * (m_render->m_shadowBuffer->GetDepth(x,y) < shadowPoint[2] + 15);
	}
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
	auto r = (2 * n * (n * l) - l).normalize();
	auto specColor = m_model->spec(uv.x,uv.y);
	auto spec = pow(std::max<double>(r.z, 0),specColor[0]);
	auto intensity = std::max(static_cast<double>(0), n * l);
	for(int i = 0; i < 3; i++)
	{
		diffColor[i] = std::min<float>(10 + diffColor[i]  * shadowVal * (0.8 * intensity + 0.1 * spec), 255);
	}
	color = diffColor;
	return false;
}

void NormalTangentMappingWithPhongReflectionAndShadowMapShader::update()
{
	IShader::update();
	auto m = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate;
	this->uniform_MShadow = m_camera->M_Shadow * m.invert();
}

void DepthShader::update()
{
	auto origPos = m_camera->position;
	IShader::update();
	m_camera->LookAt(m_light->direction);
	m_camera->SetProjectionMatrix();
	m_camera->Projection = m_camera->ProjectionOtho;
	m_camera->M_Shadow = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate;
	m_render->m_curBuffer = m_render->m_shadowBuffer;
	m_render->m_useOutPut = false;
	m_camera->position = origPos;
}

void DepthShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	IShader::vertex(faceIndex, vertIndex, position);
}

bool DepthShader::fragment(const vec3 barycentric, TGAColor& color)
{
	auto p = varying_tri*barycentric;
	color = TGAColor(255,255,255) * (p.z / m_camera->depth);
	return false;
}


