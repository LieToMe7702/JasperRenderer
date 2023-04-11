#include "core.h"
#include "render_helper.h"

void Camera::LookAt(vec3 target)
{
	auto z = (center - target).normalize();
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
			{1, 0, 0, -target.x},
			{0, 1, 0, -target.y},
			{0, 0, 1, -target.z},
			{0, 0, 0, 1}
		}
	};
	ModelView = Minv * Tr;
	this->position = target;
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
	 double near = this->m_near;
	 double far =  this->m_far;
	 double top = this->position.y + this->m_nearHalfY;
	 double bottom = this->position.y - this->m_nearHalfY;
	 double left = this->position.x - this->m_nearHalfX;
	 double right = this->position.x + this->m_nearHalfX;

	Projection = {
		{
			{2 * near / (right - left), 0, (left + right)/(left-right), 0},
			{0, 2*near/(top-bottom), (bottom + top)/(top - bottom), 0},
			{0, 0, (far+near)/(near-far), 2*near*far/(near-far)},
			{0, 0, -1, 0}
		}
	};

	 mat<4, 4> matrix_scale = {
		 {
			 {2 / (right - left), 0, 0, 0},
			 {0, 2 / (top - bottom), 0, 0},
			 {0, 0, 2 / (near - far), 0},
			 {0, 0, 0, 1}
		 }
	 };
	 mat<4, 4> matrix_tran = {
		 {
			 {1, 0, 0, -(left + right) / 2},
			 {0, 1, 0, -(bottom + top) / 2},
			 {0, 0, 1, -(near + far) / 2},
			 {0, 0, 0, 1}
		 }
	 };
	ProjectionOtho = matrix_scale * matrix_tran;
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
	RotateIt = Rotate.invert_transpose();
}

void Camera::SetParam(double near, double far, double nearHalfX, double nearHalfY)
{
	this->m_far = far;
	this->m_near = near;
	this->m_nearHalfX = nearHalfX;
	this->m_nearHalfY = nearHalfY;
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
	m_shaderList.push_back(shader);
	shader->AddRender(shared_from_this());
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
	m_shadowBuffer->Clear();
	m_zbuffer->Clear();
	if (m_outPut != nullptr)
	{
		m_outPut->BeforeOutPot();
	}
	auto camera = m_camera;
	auto height = m_screenY;
	auto width = m_screenX;
	camera->SetRotate();
	camera->SetViewPortMatrix(0, 0, width, height);
	DoDraw();

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
	m_shadowBuffer = std::make_shared<ZBuffer>(m_screenX, m_screenY);
}

void Renderer::DrawTriangle(vec4 vecs[3], std::shared_ptr<IShader> shader, std::shared_ptr<ZBuffer> buffer, std::shared_ptr<IOutPutTarget> output)
{
	vec4 pts_screen[3] = {m_camera->Viewport * vecs[0],m_camera->Viewport * vecs[1], m_camera->Viewport * vecs[2]};
	auto width = m_screenX;
	auto height = m_screenY;
	vec2 box_min(width - 1, height - 1);
	vec2 box_max(0, 0);
	auto z_buffer = buffer;
	vec2 pts_perspective_division[3];

	pts_perspective_division[0] = proj<2>(pts_screen[0] / pts_screen[0][3]);
	pts_perspective_division[1] = proj<2>(pts_screen[1] / pts_screen[1][3]);
	pts_perspective_division[2] = proj<2>(pts_screen[2] / pts_screen[2][3]);

	for (int i = 0; i < 3; i++)
	{
		box_min.x = std::min(box_min.x, pts_perspective_division[i][0]);
		box_min.y = std::min(box_min.y, pts_perspective_division[i][1]);

		box_max.x = std::max(box_max.x, pts_perspective_division[i][0]);
		box_max.y = std::max(box_max.y, pts_perspective_division[i][1]);
	}
	vec2 p;
	box_min.x = std::max(box_min.x, static_cast<double>(0));
	box_min.y = std::max(box_min.y, static_cast<double>(0));
	box_max.x = std::min(box_max.x, static_cast<double>(width - 1));
	box_max.y = std::min(box_max.y, static_cast<double>(height - 1));

	TGAColor color;
	for (p.x = static_cast<int>(box_min.x); p.x <= static_cast<int>(box_max.x); p.x++)
	{
		for (p.y = static_cast<int>(box_min.y); p.y <= static_cast<int>(box_max.y); p.y++)
		{
			auto bc_screen = barycentric(pts_perspective_division, p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

			auto bc_screen_revised = vec3(bc_screen[0] / pts_screen[0][3], bc_screen[1] / pts_screen[1][3],
			                    bc_screen[2] / pts_screen[2][3]);
			bc_screen_revised = bc_screen_revised / (bc_screen_revised.x + bc_screen_revised.y + bc_screen_revised.z);
			auto depth = vec3(vecs[0][2], vecs[1][2], vecs[2][2]) * bc_screen_revised;

			if(z_buffer->GetDepth(p.x,p.y) < depth)
			{
				if(!shader->fragment(bc_screen_revised, color))
				{
					z_buffer->SetDepth(p.x, p.y, depth);
					if(output != nullptr)
					{
						output->SetColor(p.x, p.y, color);
					}
				}
			}
		}
	}
}

void Renderer::DoDraw()
{
	for (auto shader : m_shaderList)
	{
		shader->update();
		for (int i = 0; i < m_model->nfaces(); i++)
		{
			vec4 clip_vert[3];
			for (int j = 0; j < 3; j++)
			{
				shader->vertex(i, j, clip_vert[j]);
			}
			DrawTriangle(clip_vert, shader ,m_curBuffer, m_useOutPut? m_outPut:nullptr);
		}
	}
	
}


void TGAOutPutTarget::SetColor(int x, int y, TGAColor& color)
{
	y = m_image->height() - y - 1;
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
	position = /*m_camera->Viewport **/ m_camera->Projection * m_camera->ModelView * m_camera->Rotate * position;

	varying_uv.set_col(vertIndex, m_model->uv(faceIndex, vertIndex));
	auto subPos = m_camera->Viewport * position;
	varying_tri.set_col(vertIndex,proj<3>(subPos/subPos[3]));
}

void IShader::update()
{
	m_camera->LookAt(m_camera->position);
	m_camera->SetProjectionMatrix();
	m_render->m_curBuffer = m_render->m_zbuffer;
	m_render->m_useOutPut = true;
	this->uniform_M = m_camera->Projection * m_camera->ModelView;
	this->uniform_MIT = uniform_M.invert_transpose();
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

void IShader::AddRender(std::shared_ptr<Renderer> renderer)
{
	m_render = renderer;
	AddModel(renderer->m_model);
	AddLight(renderer->m_light);
	AddCamera(renderer->m_camera);
}
