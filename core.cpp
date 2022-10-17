#include "core.h"

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

void Renderer::SetZBuffer(std::shared_ptr<ZBuffer> zBuffer)
{
	m_zbuffer = zBuffer;
}

void Renderer::SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice)
{
	m_outPut = outPutDevice;
}

void Renderer::AddModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void Renderer::DoRender()
{
	if(m_outPut != nullptr)
	{
		m_outPut->OutPut();
	}
}


void TGAOutPutTarget::SetColor(int x, int y, TGAColor color)
{
	
}

void TGAOutPutTarget::OutPut()
{
	m_image->write_tga_file(name + ".tga");
}

TGAOutPutTarget::TGAOutPutTarget(int x, int y, std::string name)
{
	m_image = std::make_shared<TGAImage>(x, y, TGAImage::RGB);
	this->name = name;
}
