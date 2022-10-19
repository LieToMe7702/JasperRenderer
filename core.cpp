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

}


void TGAOutPutTarget::SetColor(int x, int y, TGAColor color)
{
	
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

bool IShader::fragment(const vec3 barycentric, TGAColor& color)
{
	return false;
}

void IShader::AddModel(std::shared_ptr<Model> model)
{
	m_model = model;
}
