#include "core.h"
#include "render_helper.h"

void Camera::LookAt(vec3 target)
{
	auto z = (position - target).normalize();
	//std::cout << "x="<<z[0] << "y="<<z[1]<< "z=" << z[2] << std::endl;
	auto x = cross(up, z).normalize();
	auto y = cross(z, x).normalize();

	mat<4, 4> Minv{
		{
			embed<4>(x, 0),
			embed<4>(y, 0),
			embed<4>(z, 0),
			embed<4>(vec3{}, 1),
		}
	};
	mat<4, 4> Tr{
		{
			{1, 0, 0, -position.x},
			{0, 1, 0, -position.y},
			{0, 0, 1, -position.z},
			{0, 0, 0, 1}
		}
	};
	ModelView = Minv * Tr;
	this->target = target;
}

void Camera::SetViewPortMatrix(int xOffset, int yOffset, int screenWidth, int screenHeight)
{
	Viewport = {
		{
			{screenWidth / 2.0, 0, 0, xOffset + screenWidth / 2.0},
			{0, screenHeight / 2.0, 0, yOffset + screenHeight / 2.0},
			{0, 0, depth / 2.0, depth / 2.0},
			{0, 0, 0, 1}
		}
	};
}

void Camera::SetProjectionMatrix()
{
	auto sub = (this->position - this->target).normalize();
	auto coeff = sub.norm();
	if(sub.z < 0)
	{
		coeff = -coeff;
	}
	if(abs(coeff) < std::numeric_limits<double>::min())
	{
		coeff = 0;
	}
	Projection = {
		{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, -1 / coeff, 1}
		}
	};

}

void Camera::SetRotate()
{
	auto theta = rotation.y;
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
		vec3 vec = vec3{ 0, 1, 0};
		vec.normalize();
	float w = qcos;
	auto x = vec.x * qsin;
	auto y = vec.y * qsin;
	auto z = vec.z * qsin;
	auto& m = Rotate;
	m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m[0][1] = 2 * x * y - 2 * w * z;
	m[0][2] = 2 * x * z + 2 * w * y;
	m[1][0] = 2 * x * y + 2 * w * z;
	m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m[1][2] = 2 * y * z - 2 * w * x;
	m[2][0] = 2 * x * z - 2 * w * y;
	m[2][1] = 2 * y * z + 2 * w * x;
	m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m[3][0] = m[3][1] = m[3][2] = 0.0f;
	m[0][3] = m[1][3] = m[2][3] = 0.0f;
	m[3][3] = 1.0f;
}

ZBuffer::ZBuffer(int width, int height)
{
	m_buffer.resize(width * height, std::numeric_limits<double>::lowest());
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

void ZBuffer::Clear()
{
	for (double& i : m_buffer)
	{
		i = std::numeric_limits<double>::lowest();
	}
}

void Renderer::SetCamera(std::shared_ptr<Camera> camera)
{
	m_camera = camera;
}

void Renderer::SetOutPut(std::shared_ptr<IOutPutTarget> outPutDevice)
{
	m_outPut = outPutDevice;
	m_outPut->Init(m_screenX, m_screenY, "output");
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
	m_shader->AddCamera(m_camera);
}

void Renderer::AddLight(std::shared_ptr<Light> light)
{
	m_light = light;
}

void Renderer::DoRender(bool loop)
{
	if(!loop)
	{
		DoRender();
		return;
	}
	do
	{
		DoRender();
	} while (!m_outPut->NeedClose());


}

void Renderer::DoRender()
{
	if (tick_time == 0)
	{
		tick_time = clock();
	}
	auto curTime = clock();
	if(curTime - tick_time > CLOCKS_PER_SEC)
	{
		tick_time += CLOCKS_PER_SEC;
		std::cout << frame << std::endl;
		frame = 0;
	}
	m_zbuffer->Clear();
	if (m_outPut != nullptr)
	{
		m_outPut->BeforeOutPot();
	}
	auto camera = m_camera;
	auto height = m_screenY;
	auto width = m_screenX;
	camera->SetRotate();
	camera->LookAt({ 0,0,0 });
	camera->SetViewPortMatrix(0, 0, width, height);
	camera->SetProjectionMatrix();
	m_shader->update();
	for (int i = 0; i < m_model->nfaces(); i++)
	{
		vec4 clip_vert[3];
		for (int j = 0; j < 3; j++)
		{
			m_shader->vertex(i, j, clip_vert[j]);
		}
		DrawTriangle(clip_vert);
	}

	if (m_outPut != nullptr)
	{
		m_outPut->OutPut();
	}
	frame++;
}

Renderer::Renderer(int screenX, int screenY)
{
	m_screenX = screenX;
	m_screenY = screenY;
	m_zbuffer = std::make_shared<ZBuffer>(m_screenX, m_screenY);
}

void Renderer::DrawTriangle(vec4 vecs[3])
{
	auto width = m_screenX;
	auto height = m_screenY;
	vec2 box_min(width - 1, height - 1);
	vec2 box_max(0, 0);
	auto z_buffer = m_zbuffer;


	for (int i = 0; i < 3; i++)
	{
		box_min.x = std::min(box_min.x, vecs[i][0]/ vecs[i][3]);
		box_min.y = std::min(box_min.y, vecs[i][1] / vecs[i][3]);

		box_max.x = std::max(box_max.x, vecs[i][0] / vecs[i][3]);
		box_max.y = std::max(box_max.y, vecs[i][1] / vecs[i][3]);
	}
	vec2 p;
	box_min.x = std::max(box_min.x, static_cast<double>(0));
	box_min.y = std::max(box_min.y, static_cast<double>(0));
	box_max.x = std::min(box_max.x, static_cast<double>(width - 1));
	box_max.y = std::min(box_max.y, static_cast<double>(height - 1));
	vec2 vec2array[3];
	for (p.x = (int)box_min.x; p.x <= (int)box_max.x; p.x++)
	{
		for (p.y = (int)box_min.y; p.y <= (int)box_max.y; p.y++)
		{
			vec2array[0] = proj<2>(vecs[0] / vecs[0][3]);
			vec2array[1] = proj<2>(vecs[1] / vecs[1][3]);
			vec2array[2] = proj<2>(vecs[2] / vecs[2][3]);
			auto res = barycentric(vec2array, p);
			if (res.x < 0 || res.y < 0 || res.z < 0) continue;
			double z = 0;
			double w = 0;
			for (int i = 0; i < 3; i++)
			{
				z += vecs[i][2] * res[i];
				w += vecs[i][3] * res[i];
			}
			auto depth = z / w;
			if(z_buffer->GetDepth(p.x,p.y) < depth)
			{
				TGAColor color;
				if(!m_shader->fragment(res, color))
				{
					z_buffer->SetDepth(p.x, p.y, depth);
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

bool TGAOutPutTarget::NeedClose()
{
	return true;
}

void TGAOutPutTarget::BeforeOutPot()
{
}


void IShader::vertex(const int faceIndex, const int vertIndex, vec4& position)
{
	auto res = m_model->vert(faceIndex, vertIndex);
	world[vertIndex] = res;
	position = embed<4>(res);
	position = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate * position;

	varying_uv.set_col(vertIndex, m_model->uv(faceIndex, vertIndex));
}

void IShader::update()
{
	this->uniform_M = m_camera->Viewport * m_camera->Projection * m_camera->ModelView * m_camera->Rotate;
	this->uniform_MIT = this->uniform_M.invert_transpose();
}

void IShader::AddModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void IShader::AddLight(std::shared_ptr<Light> light)
{
	m_light = light;
}

void IShader::AddCamera(std::shared_ptr<Camera> camera)
{
	m_camera = camera;
}
