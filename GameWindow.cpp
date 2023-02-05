#include "GameWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	Vector4D m_light_direction;
	Vector4D m_cam_pos;
};

void GameWindow::onCreate()
{
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_cobble_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\grass.jpg");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\statue.obj");
	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	m_world_camera.setTranslation(Vector3D(0, 0, -1));

	//vertex list[] =
	//{
		//{-0.5f, -0.5f, 0.0f},	  //V1
		//{-0.5f, 0.5f, 0.0f},	  //V2
		//{0.5f, 0.5f, 0.0f},	  //V3

		//{0.5f, 0.5f, 0.0f},	  //V4
		//{0.5f, -0.5f, 0.0f},	  //V5
		//{-0.5f, -0.5f, 0.0f}	  //V6
	//};


	/*Vector3D positions[] =
	{
		{Vector3D(-0.5f, -0.5f, -0.5f)},  //V1
		{Vector3D(-0.5f, 0.5f, -0.5f)},	  //V2
		{Vector3D(0.5f, 0.5f, -0.5f)},	  //V3
		{Vector3D(0.5f, -0.5f, -0.5f)},	  //V4

		{Vector3D(0.5f, -0.5f, 0.5f)},	  //V1
		{Vector3D(0.5f, 0.5f, 0.5f)},	  //V2
		{Vector3D(-0.5f, 0.5f, 0.5f)},	  //V3
		{Vector3D(-0.5f, -0.5f, 0.5f)}	  //V4
	};

	Vector2D texcoords[] =
	{
		{Vector2D(0.0f, 0.0f)},     //V1
		{Vector2D(0.0f, 1.0f)},	  //V2
		{Vector2D(1.0f, 0.0f)},	  //V3
		{Vector2D(1.0f, 1.0f)},	  //V4
	};

	vertex vertex_list[] =
	{
		{ positions[0], texcoords[1] },
		{ positions[1], texcoords[0] },
		{ positions[2], texcoords[2] },
		{ positions[3], texcoords[3] },

		{ positions[4], texcoords[1] },
		{ positions[5], texcoords[0] },
		{ positions[6], texcoords[2] },
		{ positions[7], texcoords[3] },

		{ positions[1], texcoords[1] },
		{ positions[6], texcoords[0] },
		{ positions[5], texcoords[2] },
		{ positions[2], texcoords[3] },

		{ positions[7], texcoords[1] },
		{ positions[0], texcoords[0] },
		{ positions[3], texcoords[2] },
		{ positions[4], texcoords[3] },

		{ positions[3], texcoords[1] },
		{ positions[2], texcoords[0] },
		{ positions[5], texcoords[2] },
		{ positions[4], texcoords[3] },

		{ positions[7], texcoords[1] },
		{ positions[6], texcoords[0] },
		{ positions[1], texcoords[2] },
		{ positions[0], texcoords[3] },
	};

	
	UINT list_size = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	UINT index_list_size = ARRAYSIZE(index_list);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, index_list_size);*/

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	//m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), list_size, shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant con;

	m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&con, sizeof(constant));
}

void GameWindow::onUpdate()
{
	//Window::onUpdate();
	InputSystem::get()->update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_constant_buffer);


	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_pixel_shader, m_cobble_tex);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh ->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta)?((m_new_delta - m_old_delta) / 1000.0f):0;
}

GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::update()
{

	constant con;
	m_delta_position += m_delta_time / 10.0f;
	if (m_delta_position > 1.0f)
		m_delta_position = 0;

	Matrix4x4 temp;
	Matrix4x4 m_light_rot;
	m_light_rot.setIdentity();
	m_light_rot.setRotaionY(m_light_rotaion_y);
	m_light_rotaion_y += 0.707f * m_delta_time;
	con.m_light_direction = m_light_rot.getZDirection();
	//con.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_position));

	m_delta_scale += m_delta_time / 0.55f;
	//con.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_position));
	//con.m_world *= temp;

	//con.m_world.setScale(Vector3D(m_cube_scale, m_cube_scale, m_cube_scale));
	//temp.setIdentity();
	//temp.setRotaionZ(0.0f);
	//con.m_world *= temp;

	//temp.setIdentity();
	//temp.setRotaionY(m_rotaion_y);
	//con.m_world *= temp;

	//temp.setIdentity();
	//temp.setRotaionX(m_rotaion_x);
	//con.m_world *= temp;

	con.m_world.setIdentity();
	Matrix4x4 camera;
	camera.setIdentity();



	temp.setIdentity();
	temp.setRotaionX(m_rotaion_x);
	camera *= temp;

	temp.setIdentity();
	temp.setRotaionY(m_rotaion_y);
	camera *= temp;
	Vector3D new_pos = m_world_camera.getTranslation() + camera.getZDirection() * (m_forward * 0.01f);
	new_pos = new_pos + camera.getXDirection() * (m_sideward * 0.01f);

	camera.setTranslation(new_pos);
	con.m_cam_pos = new_pos;
	m_world_camera = camera;
	camera.inverse();

	con.m_view = camera;
	/**con.m_projection.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		 4.0f
	);*/
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	con.m_projection.setPerspectiveView(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &con);
}

void GameWindow::onDestroy()
{
	Window::onDestroy();

	//GraphicsEngine::get()->release();
}

void GameWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void GameWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void GameWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rotaion_x += 1.9f * m_delta_time;
		m_forward = 1.0f;
	}

	else if (key == 'S')
	{
		//m_rotaion_x -= 1.9f * m_delta_time;
		m_forward = -1.0f;
	}

	if (key == 'A')
	{
		//m_rotaion_y += 1.9f * m_delta_time;
		m_sideward = -1.0f;
	}

	else if (key == 'D')
	{
		//m_rotaion_y -= 1.9f * m_delta_time;
		m_sideward = 1.0f;
	}
}

void GameWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_sideward = 0.0f;

}

void GameWindow::onMouseMove(const Point& mouse_position)
{
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	m_rotaion_x += (mouse_position.m_y - (height / 2.0f)) * m_delta_time * 0.15f;
	m_rotaion_y += (mouse_position.m_x - (width / 2.0f)) * m_delta_time * 0.15f;
	InputSystem::get()->setInputPosition(Point(width / 2.0f, height / 2.0f));
}

void GameWindow::onLeftMouseDown(const Point& mouse_position)
{
	m_cube_scale = 0.5f;
}

void GameWindow::onLeftMouseUp(const Point& mouse_position)
{
	m_cube_scale = 1.0f;
}

void GameWindow::onRightMouseDown(const Point& mouse_position)
{
	m_cube_scale = 2.0f;
}

void GameWindow::onRightMouseUp(const Point& mouse_position)
{
	m_cube_scale = 1.0f;
}
