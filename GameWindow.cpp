#include "GameWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include <DirectXMath.h>

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
	Vector4D m_light_pos = Vector4D(0,1,0,0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

void GameWindow::onCreate()
{
	InputSystem::get()->addListener(this);
	game_state = true;
	InputSystem::get()->showCursor(false);

	m_cobble_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\grass.jpg");
	m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	m_brick_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_earth_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_specular_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	m_sky_sphere = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_torus_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\torus.obj");
	m_suzanne_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	m_plane_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\plane.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	m_world_camera.setTranslation(Vector3D(0, 0, -1));

	m_material = GraphicsEngine::get()->createMaterial(L"PLVS.hlsl", L"PLPS.hlsl");
	m_material->addTexture(m_brick_tex);
	m_material->setCullMode(CULL_MODE_BACK);
	m_sky_material = GraphicsEngine::get()->createMaterial(L"PLVS.hlsl", L"SkyShader.hlsl");
	m_sky_material->addTexture(m_sky_tex);
	m_sky_material->setCullMode(CULL_MODE_FRONT);

	m_earth_material = GraphicsEngine::get()->createMaterial(m_material);
	m_earth_material->addTexture(m_earth_tex);
	m_earth_material->setCullMode(CULL_MODE_BACK);
	m_b_material = GraphicsEngine::get()->createMaterial(m_material);
	m_b_material->addTexture(m_wall_tex);
	m_b_material->setCullMode(CULL_MODE_BACK);
}

void GameWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	this->render();


}

GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::update()
{
	updateCamera();
	updateLight();
	updateSky();
}

void GameWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();
	drawMesh(m_sky_sphere, m_sky_material);

	for (int i = 0; i < 3; i++) {
		updateModel(Vector3D(4, 2, -4 + 4 * i), m_earth_material);
		drawMesh(m_sky_sphere, m_earth_material);

		updateModel(Vector3D(-4, 2, -4 + 4 * i), m_b_material);
		drawMesh(m_torus_mesh, m_b_material);

		updateModel(Vector3D(0, 2, -4 + 4 * i), m_material);
		drawMesh(m_suzanne_mesh, m_material);
	}

	updateModel(Vector3D(0, 0, 0), m_b_material);
	drawMesh(m_plane_mesh, m_b_material);


	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void GameWindow::drawMesh(const MPtr& mesh, const MTPtr& material)
{
	GraphicsEngine::get()->setMaterial(material);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}

void GameWindow::updateCamera()
{
	Matrix4x4 camera, temp;
	camera.setIdentity();



	temp.setIdentity();
	temp.setRotaionX(m_rotaion_x);
	camera *= temp;

	temp.setIdentity();
	temp.setRotaionY(m_rotaion_y);
	camera *= temp;
	Vector3D new_pos = m_world_camera.getTranslation() + camera.getZDirection() * (m_forward * speed);
	new_pos = new_pos + camera.getXDirection() * (m_sideward * speed);
	new_pos = new_pos + camera.getYDirection() * (jump);

	camera.setTranslation(new_pos);
	m_world_camera = camera;
	camera.inverse();

	m_view_camera = camera;
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	m_projection_camera.setPerspectiveView(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void GameWindow::updateModel(Vector3D pos, const MTPtr& material)
{
	constant con;

	Matrix4x4 m_light_rot;
	m_light_rot.setIdentity();
	m_light_rot.setRotaionY(m_light_rotaion_y);
	//con.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_position));

	con.m_world.setIdentity();
	con.m_world.setTranslation(pos);
	con.m_view = m_view_camera;
	con.m_projection = m_projection_camera;
	con.m_cam_pos = m_world_camera.getTranslation();

	float distance_origin = 1.0f;
	con.m_light_pos = m_light_pos;
	con.m_light_radius = m_light_radius;

	con.m_light_direction = m_light_rot.getZDirection();
	material->setData(&con, sizeof(constant));
}

void GameWindow::updateSky()
{
	constant con;
	con.m_world.setIdentity();
	con.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	con.m_world.setTranslation(m_world_camera.getTranslation());
	con.m_view = m_view_camera;
	con.m_projection = m_projection_camera;

	m_sky_material->setData(&con, sizeof(constant));
}

void GameWindow::updateLight()
{
	m_light_rotaion_y += 1.57f * m_delta_time;
	float distance_origin = 3.0f;
	m_light_pos = Vector4D(cos(m_light_rotaion_y) * distance_origin, 1.0f, sin(m_light_rotaion_y) * distance_origin, 1.0f);
}

void GameWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
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

void GameWindow::onSize()
{
	RECT rc = this -> getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
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

	else if (key == 'A')
	{
		//m_rotaion_y += 1.9f * m_delta_time;
		m_sideward = -1.0f;
	}

	else if (key == 'D')
	{
		//m_rotaion_y -= 1.9f * m_delta_time;
		m_sideward = 1.0f;
	}

	else if (key == 16)
	{
		speed = 3.0f;
	}

	else if (key == 32)
	{
		jump = 1.0f;
	}
	else if (key == 'O')
	{
		m_light_radius -= 10.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		m_light_radius += 10.0f * m_delta_time;
	}
}

void GameWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_sideward = 0.0f;
	speed = 0.2f;
	jump = -1.0f;
	jump = 0.0f;

	if (key == 'V')
	{
		game_state = !(game_state);
		InputSystem::get()->showCursor(!game_state);
	}

	else if (key == 'F')
	{
		fullscreen = (fullscreen) ? false : true;
		RECT screen_size = this->getScreenSize();
		m_swap_chain->setFullScreen(fullscreen, screen_size.right, screen_size.bottom);
		
	}

}

void GameWindow::onMouseMove(const Point& mouse_position)
{
	if(!game_state) return;
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	m_rotaion_x += (mouse_position.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rotaion_y += (mouse_position.m_x - (width / 2.0f)) * m_delta_time * 0.1f;
	InputSystem::get()->setInputPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
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
