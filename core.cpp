#include "core.h"

#include "render_helper.h"

void Camera::LookAt(vec3 target)
{
}

void Camera::SetViewPortMatrix(int xOffset, int yOffset, int screenWidth, int screenHeight)
{
}

void Camera::SetProjectionMatrix(float coeff)
{
}

ZBuffer::ZBuffer(int width, int height)
{
	m_buffer.resize(width * height, std::numeric_limits<double>::min());
	m_width = width;
	m_height = height;
}

double ZBuffer::GetDepth(int x, int y)
{
	return m_buffer[y * m_width + x];
}

void ZBuffer::SetDepth(int x, int y, double val)
{
	m_buffer[y * m_width + x] = val;
}

void Renderer::SetCamera(std::shared_ptr<Camera> camera)
{
	m_camera = camera;
}

void Renderer::SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice)
{
	m_outPut = outPutDevice;
	m_outPut->Init(m_screeX, m_screenY, "output");
}

void Renderer::AddModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void Renderer::AddShader(std::shared_ptr<IShader> shader)
{
	m_shader = shader;
	m_shader->AddModel(m_model);
	m_shader->AddLight(m_light);
}

void Renderer::AddLight(std::shared_ptr<Light> light)
{
	m_light = light;
}

void Renderer::DoRender()
{
	for(int i = 0; i < m_model->nfaces();i++)
	{
		vec4 clip_vert[3];
		for(int j = 0; j < 3;j++)
		{
			m_shader->vertex(i, j, clip_vert[j]);
		}
		DrawTriangle(clip_vert);
	}

	if(m_outPut != nullptr)
	{
		m_outPut->OutPut();
	}
}

Renderer::Renderer(int screenX, int screenY)
{
	m_screeX = screenX;
	m_screenY = screenY;
	m_zbuffer = std::make_shared<ZBuffer>(m_screeX, m_screenY);
}

void Renderer::DrawTriangle(vec4 vecs[3])
{
	auto width = m_screeX;
	auto height = m_screenY;
	vec2 box_min(width - 1, height - 1);
	vec2 box_max(0, 0);
	vec3 points[3];
	auto depth = 256;
	auto z_buffer = m_zbuffer;
	for(int i = 0; i < 3; i++)
	{
		auto world_coord = proj<3>(vecs[i]);
		points[i] = vec3((int)((world_coord.x + 1) * width / 2),
			(int)((world_coord.y + 1) * height / 2), (int)((world_coord.z + 1) * depth / 2));
	}

	for (int i = 0; i < 3; i++)
	{
		box_min.x = std::min(box_min.x, points[i].x);
		box_min.y = std::min(box_min.y, points[i].y);

		box_max.x = std::max(box_max.x, points[i].x);
		box_max.y = std::max(box_max.y, points[i].y);
	}
	vec2 p;
	box_min.x = std::max(box_min.x, static_cast<double>(0));
	box_min.y = std::max(box_min.y, static_cast<double>(0));
	box_max.x = std::min(box_max.x, static_cast<double>(width - 1));
	box_max.y = std::min(box_max.y, static_cast<double>(height - 1));
	for (p.x = (int)box_min.x; p.x <= (int)box_max.x; p.x++)
	{
		for (p.y = (int)box_min.y; p.y <= (int)box_max.y; p.y++)
		{
			auto res = barycentric(points, p);
			if (res.x < 0 || res.y < 0 || res.z < 0) continue;
			double z = 0;
			for (int i = 0; i < 3; i++)
			{
				z += points[i].z * res[i];
			}
			if(z_buffer->GetDepth(p.x,p.y) < z)
			{
				TGAColor color;
				if(m_shader->fragment(res, color))
				{
					z_buffer->SetDepth(p.x, p.y, z);
					m_outPut->SetColor(p.x, p.y, color);
				}
			}
		}
	}
}


void TGAOutPutTarget::SetColor(int x, int y, TGAColor& color)
{
	m_image->set(x, y, color);
}

void TGAOutPutTarget::OutPut()
{
	m_image->write_tga_file(name + ".tga");
}


void TGAOutPutTarget::Init(int x, int y, std::string name)
{
	m_image = std::make_shared<TGAImage>(x, y, TGAImage::RGB);
	this->name = name;
}


void IShader::AddModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void IShader::AddLight(std::shared_ptr<Light> light)
{
	m_light = light;
}
