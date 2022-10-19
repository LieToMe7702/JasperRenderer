#include "core.h"
#include "shader.h"

void NormalShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	uvs[vertIndex] = m_model->uv(faceIndex, vertIndex);
	world[vertIndex] = res;
	position = embed<4>(res);
}

bool NormalShader::fragment(const vec3 barycentric, TGAColor& color)
{
	vec3 light_dir(0, 0, 1);

	auto& diff = m_model->diffuse();
	auto newUV = uvs[0] * barycentric[0] + uvs[1] * barycentric[1] + uvs[2] * barycentric[2];
	auto diffU = diff.width() * newUV.x;
	auto diffY = diff.height() * newUV.y;
	auto diffColor = diff.get(diffU, diffY);
	auto normal = cross((world[1] - world[0]), (world[2] - world[0]));
	normal.normalize();
	auto intensity = normal * light_dir;

	diffColor[0] *= intensity;
	diffColor[1] *= intensity;
	diffColor[2] *= intensity;
	color = diffColor;
	return intensity > 0;
}
