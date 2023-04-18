#define NOMINMAX
#include "GameWindow.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "VMesh.h"
#include "SwapChain.h"
#include <DirectXMath.h>
#include <Windows.h>

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
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
};

enum IntersectionSide { NONE, LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };

struct BoundingBox {
	Vector3D minPoint;
	Vector3D maxPoint;
};
std::vector<BoundingBox> boundingBoxList;
BoundingBox boxBoundingBox;
BoundingBox playerBoundingBox;
BoundingBox planeBoundingBox;
BoundingBox SkyBoundingBoxl;


bool playerScale = false;
std::vector<Vector4D> projectiles;
std::vector<Matrix4x4> projectilesMatrix;
std::vector<BoundingBox> projectilesBoundingBoxes;
float projectileSpeed = 1.5f;
Matrix4x4 cam;
float enemyHealth = 100;
bool bullFlag = false;
// Define a function to calculate the bounding box of a list of vertices
BoundingBox CalculateBoundingBox(const std::vector<Vector3D>& vertices, Vector3D& minPoint, Vector3D& maxPoint)
{
	// Initialize the minimum and maximum points to the first vertex position
	minPoint = vertices[0];
	maxPoint = vertices[0];

	// Loop through all vertices and update the minimum and maximum points as necessary
	for (const auto& vertex : vertices) {
		if (vertex.m_x < minPoint.m_x)
			minPoint.m_x = vertex.m_x;
		if (vertex.m_y < minPoint.m_y)
			minPoint.m_y = vertex.m_y;
		if (vertex.m_z < minPoint.m_z)
			minPoint.m_z = vertex.m_z;

		if (vertex.m_x > maxPoint.m_x)
			maxPoint.m_x = vertex.m_x;
		if (vertex.m_y > maxPoint.m_y)
			maxPoint.m_y = vertex.m_y;
		if (vertex.m_z > maxPoint.m_z)
			maxPoint.m_z = vertex.m_z;
	}
	BoundingBox boundingBoxTemp;
	boundingBoxTemp.minPoint = minPoint;
	boundingBoxTemp.maxPoint = maxPoint;

	return boundingBoxTemp;
}

//bool CheckBoundingBoxCollision(BoundingBox box1, BoundingBox box2)
//{
//	// Check for separation along X axis
//	if (box1.maxPoint.m_x < box2.minPoint.m_x || box1.minPoint.m_x > box2.maxPoint.m_x)
//		return false;
//
//	// Check for separation along Y axis
//	if (box1.maxPoint.m_y < box2.minPoint.m_y || box1.minPoint.m_y > box2.maxPoint.m_y)
//		return false;
//
//	// Check for separation along Z axis
//	if (box1.maxPoint.m_z < box2.minPoint.m_z || box1.minPoint.m_z > box2.maxPoint.m_z)
//		return false;
//
//	// If there is no separation along any axis, the boxes intersect
//	return true;
//}

IntersectionSide CheckBoundingBoxCollision(BoundingBox b1, BoundingBox b2)
{
    float dist1, dist2;
    IntersectionSide side = NONE;

    // Check X axis
    dist1 = b1.maxPoint.m_x - b2.minPoint.m_x;
    dist2 = b2.maxPoint.m_x - b1.minPoint.m_x;
    if (dist1 < 0.0f || dist2 < 0.0f)
    {
        return NONE; // No intersection
    }
    else if (dist1 < dist2)
    {
        side = LEFT;
    }
    else
    {
        side = RIGHT;
    }

    // Check Y axis
    dist1 = b1.maxPoint.m_y - b2.minPoint.m_y;
    dist2 = b2.maxPoint.m_y - b1.minPoint.m_y;
    if (dist1 < 0.0f || dist2 < 0.0f)
    {
        return NONE; // No intersection
    }
    else if (dist1 < dist2)
    {
        side = TOP;
    }
    else
    {
        side = BOTTOM;
    }

    // Check Z axis
    dist1 = b1.maxPoint.m_z - b2.minPoint.m_z;
    dist2 = b2.maxPoint.m_z - b1.minPoint.m_z;
    if (dist1 < 0.0f || dist2 < 0.0f)
    {
        return NONE; // No intersection
    }
    else if (dist1 < dist2)
    {
        side = FRONT;
    }
    else
    {
        side = BACK;
    }

    return side;
}

void GameWindow::onCreate()
{
	InputSystem::get()->addListener(this);
	game_state = true;
	InputSystem::get()->showCursor(false);

	m_cobble_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\grass.jpg");
	m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\asteroid.jpg");
	m_earth_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_plane_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stone2.jpg");
	m_specular_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	m_sky_sphere = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_plane_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\terrain.obj");
	m_bullet_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	m_box_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wooden-box.jpg");
	m_box_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\box.obj");

	//m_barrel_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	//m_brick_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	//m_windows_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_windows.jpg");
	//m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");
	//m_house_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\house.obj");




	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	m_world_camera.setTranslation(Vector3D(0, 0, -1));

	m_material = GraphicsEngine::get()->createMaterial(L"PLVS.hlsl", L"PLPS.hlsl");
	m_material->addTexture(m_bricks_tex);
	m_material->setCullMode(CULL_MODE_BACK);
	m_sky_material = GraphicsEngine::get()->createMaterial(L"PLVS.hlsl", L"SkyShader.hlsl");
	m_sky_material->addTexture(m_sky_tex);
	m_sky_material->setCullMode(CULL_MODE_FRONT);


	m_plane_material = GraphicsEngine::get()->createMaterial(m_material);
	m_plane_material->addTexture(m_plane_tex);
	m_plane_material->setCullMode(CULL_MODE_BACK);



	//m_barrel_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_barrel_material->addTexture(m_barrel_tex);
	//m_barrel_material->setCullMode(CULL_MODE_BACK);

	//m_brick_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_brick_material->addTexture(m_brick_tex);
	//m_brick_material->setCullMode(CULL_MODE_BACK);

	//m_windows_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_windows_material->addTexture(m_windows_tex);
	//m_windows_material->setCullMode(CULL_MODE_BACK);

	//m_wood_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_wood_material->addTexture(m_wood_tex);
	//m_wood_material->setCullMode(CULL_MODE_BACK);

	m_box_material = GraphicsEngine::get()->createMaterial(m_sky_material);
	m_box_material->addTexture(m_box_tex);
	m_box_material->setCullMode(CULL_MODE_BACK);

	m_material_list.reserve(32);
}


void GameWindow::onUpdate()
{
	if(CheckBoundingBoxCollision(planeBoundingBox, playerBoundingBox) != NONE)
		m_upward -= 0.05f;
	else
		m_upward = 0.0f;

	for (int i = 0; i < projectilesBoundingBoxes.size(); i++) {
		if (CheckBoundingBoxCollision(projectilesBoundingBoxes[i], boxBoundingBox) != NONE) {
			enemyHealth -= 25;
			projectilesBoundingBoxes.erase(projectilesBoundingBoxes.begin() + i);
			projectiles.erase(projectiles.begin() + i);
			projectilesMatrix.erase(projectilesMatrix.begin() + i);
		}
		//if (CheckBoundingBoxCollision(projectilesBoundingBoxes[i], boxBoundingBox) != NONE) {
		//	projectilesBoundingBoxes.erase(projectilesBoundingBoxes.begin() + i);
		//	projectiles.erase(projectiles.begin() + i);
		//	projectilesMatrix.erase(projectilesMatrix.begin() + i);
		//}
	}

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

Vector3D getTrasformedVertex(Vector3D vector, Matrix4x4 light_rot) {
	// Define your world transformation matrix
	DirectX::XMMATRIX worldMatrix = DirectX::XMMATRIX(
		light_rot.m_matrix[0][0], light_rot.m_matrix[0][1], light_rot.m_matrix[0][2], light_rot.m_matrix[0][3],
		light_rot.m_matrix[1][0], light_rot.m_matrix[1][1], light_rot.m_matrix[1][2], light_rot.m_matrix[1][3],
		light_rot.m_matrix[2][0], light_rot.m_matrix[2][1], light_rot.m_matrix[2][2], light_rot.m_matrix[2][3],
		light_rot.m_matrix[3][0], light_rot.m_matrix[3][1], light_rot.m_matrix[3][2], light_rot.m_matrix[3][3]);
	// Define your vertex
	DirectX::XMFLOAT3 vertex = DirectX::XMFLOAT3(vector.m_x, vector.m_y, vector.m_z);

	// Convert the vertex to a vector
	DirectX::XMVECTOR vertexVector = DirectX::XMLoadFloat3(&vertex);

	// Multiply the vertex by the world transformation matrix
	DirectX::XMVECTOR transformedVertex = XMVector3Transform(vertexVector, worldMatrix);

	// Convert the result back to a XMFLOAT4
	DirectX::XMFLOAT3 transformedVertexFloat3;
	DirectX::XMStoreFloat3(&transformedVertexFloat3, transformedVertex);

	return Vector3D(transformedVertexFloat3.x, transformedVertexFloat3.y, transformedVertexFloat3.z);
}
void GameWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	//m_material_list.clear();
	//m_material_list.push_back(m_barrel_material);
	//m_material_list.push_back(m_brick_material);
	//m_material_list.push_back(m_windows_material);
	//m_material_list.push_back(m_wood_material);
	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		updateModel(Vector3D(-14.0f +14.0f * i, 0, -14.0f + 14.0f * j), m_material_list);
	//		drawMesh(m_house_mesh, m_material_list);
	//	}
	//}

	std::list<Vector3D> objectList;
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 2; j++) {
	//		m_material_list.clear();
	//		m_material_list.push_back(m_box_material);
	//		Vector3D boxPos1 = Vector3D(-10 + 10 * i, 0, -7 + 7 * j);
	//		objectList.push_back(boxPos1);
	//		updateModel(boxPos1, m_material_list);
	//		drawMesh(m_box_mesh, m_material_list);
	//		Vector3D boxPos2 = Vector3D((-10 + 10 * i) - 0.8f, 0, -7 + 7 * j);
	//		objectList.push_back(boxPos2);
	//		updateModel(boxPos2, m_material_list);
	//		drawMesh(m_box_mesh, m_material_list);
	//	}
	//}
	std::vector<Vector3D> vertices;
	m_material_list.clear();
	m_material_list.push_back(m_plane_material);
	Vector3D planePos = Vector3D(0, 0, 0);
	objectList.push_back(planePos);
	updateModel(m_plane_mesh, planePos, m_material_list);
	for (int i = 0; i < m_plane_mesh.get()->v_list.size(); i++) {
		Vector3D a = Vector3D(m_plane_mesh.get()->v_list[i].m_pos.m_x, m_plane_mesh.get()->v_list[i].m_pos.m_y+2, m_plane_mesh.get()->v_list[i].m_pos.m_z);
		a = getTrasformedVertex(a, transMat);
		vertices.push_back(a);
	}
	Vector3D min2, max2;
	planeBoundingBox = CalculateBoundingBox(vertices, min2, max2);
	drawMesh(m_plane_mesh, m_material_list);

	if (enemyHealth > 0) {
		m_material_list.clear();
		m_material_list.push_back(m_box_material);
		Vector3D boxPos = Vector3D(0, 0, 0);
		vertices.clear();
		updateModel(m_box_mesh, boxPos, m_material_list);
		for (int i = 0; i < m_box_mesh.get()->v_list.size(); i++) {
			Vector3D a = Vector3D(m_box_mesh.get()->v_list[i].m_pos.m_x, m_box_mesh.get()->v_list[i].m_pos.m_y, m_box_mesh.get()->v_list[i].m_pos.m_z);
			a = getTrasformedVertex(a, transMat);
			vertices.push_back(a);
		}
		Vector3D min, max;
		boxBoundingBox = CalculateBoundingBox(vertices, min, max);
		drawMesh(m_box_mesh, m_material_list);
	}
	else {
		boxBoundingBox.minPoint = Vector3D(NULL, NULL, NULL);
		boxBoundingBox.maxPoint = Vector3D(NULL, NULL, NULL);
	}

	playerScale = 1;
	m_material_list.clear();
	m_material_list.push_back(m_material);
	Vector3D playerPos = Vector3D(m_world_camera.getTranslation().m_x - larlar, m_world_camera.getTranslation().m_y, m_world_camera.getTranslation().m_z);
	vertices.clear();
	updateModel(m_sky_sphere, playerPos, m_material_list);
	for (int i = 0; i < m_sky_sphere.get()->v_list.size(); i++) {
		Vector3D a = Vector3D(m_sky_sphere.get()->v_list[i].m_pos.m_x, m_sky_sphere.get()->v_list[i].m_pos.m_y, m_sky_sphere.get()->v_list[i].m_pos.m_z);
		a = getTrasformedVertex(a, transMat);
		vertices.push_back(a);
	}
	Vector3D min1, max1;
	drawMesh(m_sky_sphere, m_material_list);
	playerBoundingBox = CalculateBoundingBox(vertices, min1, max1);
	
	bullFlag = true;
	for (int i = 0; i < projectiles.size(); i++)
	{
		m_material_list.clear();
		m_material_list.push_back(m_material);
		Vector3D projectilePos = Vector3D(projectiles[i].m_x, projectiles[i].m_y, projectiles[i].m_z);
		projectilePos = projectilePos + (projectilesMatrix[i].getZDirection() * projectileSpeed);
		projectiles[i] = projectilePos;
		projectiles[i].m_w += 1;
		vertices.clear();
		
		updateModel(m_bullet_mesh, projectilePos, m_material_list);
		for (int i = 0; i < m_bullet_mesh.get()->v_list.size(); i++) {
			Vector3D a = Vector3D(m_bullet_mesh.get()->v_list[i].m_pos.m_x, m_bullet_mesh.get()->v_list[i].m_pos.m_y - 10, m_bullet_mesh.get()->v_list[i].m_pos.m_z);
			a = getTrasformedVertex(a, transMat);
			vertices.push_back(a);
		}
		Vector3D min3, max3;
		drawMesh(m_bullet_mesh, m_material_list);
		projectilesBoundingBoxes[i] = CalculateBoundingBox(vertices, min3, max3);
	}
	bullFlag = false;


	m_material_list.clear();
	m_material_list.push_back(m_sky_material);
	drawMesh(m_sky_sphere, m_material_list);

	//for (int i = 0; i < 3; i++) {
	//	updateModel(Vector3D(4, 2, -4 + 4 * i), m_earth_material);
	//	drawMesh(m_sky_sphere, m_earth_material);

	//	updateModel(Vector3D(-4, 2, -4 + 4 * i), m_b_material);
	//	drawMesh(m_torus_mesh, m_b_material);

	//	updateModel(Vector3D(0, 2, -4 + 4 * i), m_material);
	//	drawMesh(m_suzanne_mesh, m_material);
	//}

	//updateModel(Vector3D(0, 0, 0), m_b_material);
	//drawMesh(m_plane_mesh, m_b_material);


	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;

	// Assume that the bounding box has already been calculated

// Define the vertices of the bounding box
	DirectX::XMFLOAT3 vertices1[] = {
		{playerBoundingBox.minPoint.m_x, playerBoundingBox.minPoint.m_y, playerBoundingBox.minPoint.m_z},
		{playerBoundingBox.maxPoint.m_x, playerBoundingBox.minPoint.m_y, playerBoundingBox.minPoint.m_z},
		{playerBoundingBox.maxPoint.m_x, playerBoundingBox.maxPoint.m_y, playerBoundingBox.minPoint.m_z},
		{playerBoundingBox.minPoint.m_x, playerBoundingBox.maxPoint.m_y, playerBoundingBox.minPoint.m_z},
		{playerBoundingBox.minPoint.m_x, playerBoundingBox.minPoint.m_y, playerBoundingBox.maxPoint.m_z},
		{playerBoundingBox.maxPoint.m_x, playerBoundingBox.minPoint.m_y, playerBoundingBox.maxPoint.m_z},
		{playerBoundingBox.maxPoint.m_x, playerBoundingBox.maxPoint.m_y, playerBoundingBox.maxPoint.m_z},
		{playerBoundingBox.minPoint.m_x, playerBoundingBox.maxPoint.m_y, playerBoundingBox.maxPoint.m_z},
	};

	// Define the indices of the bounding box
	UINT indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
	};

	// Create the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices1;

	ID3D11Buffer* vertexBuffer = nullptr;
	HRESULT hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Create the index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * 24;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices;

	ID3D11Buffer* indexBuffer = nullptr;
	hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->m_device_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->m_device_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the primitive topology
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Draw the bounding box
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->m_device_context->DrawIndexed(24, 0, 0);

	// Clean up
	vertexBuffer->Release();
	indexBuffer->Release();

}

void GameWindow::drawMesh(const MPtr& mesh, const std::vector<MTPtr>& material_list)
{

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t i = 0; i < mesh->getMaterialPlaceNum(); i++) {
		if (i >= material_list.size())
			break;

		MaterialPlace material = mesh->getMaterialPlace(i);
		GraphicsEngine::get()->setMaterial(material_list[i]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(material.num, 0, material.start);
	}
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
	Vector3D new_pos = m_world_camera.getTranslation() + camera.getXDirection() * (m_sideward * speed);
	new_pos = new_pos + camera.getZDirection() * (m_forward * speed);
	new_pos = new_pos + camera.getYDirection() * (m_upward * speed);
	cam = camera;
	camera.setTranslation(new_pos);
	m_world_camera = camera;
	camera.inverse();
	m_view_camera = camera;
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	m_projection_camera.setPerspectiveView(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}


void GameWindow::updateModel(MPtr mesh, Vector3D pos, const std::vector<MTPtr>& material_list)
{
	constant con;

	m_light_rot.setIdentity();
	m_light_rot.setRotaionY(m_light_rotaion_y);
	//con.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_position));

	con.m_world.setIdentity();
	if (bullFlag)
		con.m_world.setScale(Vector3D(0.1f, 0.1f, 0.1f));
	con.m_world.setTranslation(pos);
	con.m_view = m_view_camera;
	con.m_projection = m_projection_camera;
	con.m_cam_pos = m_world_camera.getTranslation();

	float distance_origin = 1.0f;
	con.m_light_pos = m_light_pos;
	con.m_light_radius = m_light_radius;

	con.m_light_direction = m_light_rot.getZDirection();
	for (size_t i = 0; i < material_list.size(); i++) {
		material_list[i]->setData(&con, sizeof(constant));
	}
	world = con.m_world;
	transMat = con.m_world;
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
	//m_light_rotaion_y += 1.57f * m_delta_time;
	m_light_rotaion_y = 1;
	float distance_origin = 3.0f;
	//m_light_pos = Vector4D(cos(m_light_rotaion_y) * distance_origin, 1.0f, sin(m_light_rotaion_y) * distance_origin, 1.0f);
	m_light_pos = Vector4D(180, 140, 70, 1.0f);
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
		if(CheckBoundingBoxCollision(playerBoundingBox, boxBoundingBox) == FRONT) {
			//m_rotaion_x += 1.9f * m_delta_time;
			m_forward = 0.0f;
		}
		else
			m_forward = 1.0f;
	}

	else if (key == 'S')
	{
		if (CheckBoundingBoxCollision(playerBoundingBox, boxBoundingBox) == BACK) {
			//m_rotaion_x -= 1.9f * m_delta_time;
			m_forward = 0.0f;
		}
		else
			m_forward = -1.0f;
	}

	else if (key == 'A')
	{
		if (CheckBoundingBoxCollision(boxBoundingBox, playerBoundingBox) == LEFT) {
			//m_rotaion_x += 1.9f * m_delta_time;
			m_sideward = 0.0f;
		}
		else
			m_sideward = -1.0f;
	}

	else if (key == 'D')
	{
		if (CheckBoundingBoxCollision(boxBoundingBox, playerBoundingBox) == RIGHT) {
			//m_rotaion_x += 1.9f * m_delta_time;
			m_sideward = 0.0f;
		}
		else
			m_sideward = 1.0f;
	}

	else if (key == 16)
	{
		speed = 3.0f;
	}

	//else if (key == 32)
	//{
	//	m_upward = 1.0f;
	//}
	else if (key == 'O')
	{
		m_light_radius -= 10.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		m_light_radius += 10.0f * m_delta_time;
	}
	else if (key == 'Y')
	{
		larlar += 1.0f;
	}
	else if (key == 'U')
	{
		larlar -= 1.0f;
	}
}

void GameWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_sideward = 0.0f;
	speed = 0.2f;
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

	else if (key == 'E')
	{

	}

	else if (key == 'N') {
		enemyHealth += 100;
	}


	else if (key == 32)
	{
		if(m_upward == 0.0f)
			m_upward = 0.5f;
		else {
			if (m_upward == -1.0f)
				m_upward = 0.0f;
			//m_upward = -1.0f;
		}
	}

}

void GameWindow::onMouseMove(const Point& mouse_position)
{
	if (!game_state) return;
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	m_rotaion_x += (mouse_position.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rotaion_y += (mouse_position.m_x - (width / 2.0f)) * m_delta_time * 0.1f;
	InputSystem::get()->setInputPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void GameWindow::onLeftMouseDown(const Point& mouse_position)
{
	projectiles.push_back(Vector3D(m_world_camera.getTranslation().m_x, m_world_camera.getTranslation().m_y, m_world_camera.getTranslation().m_z));
	projectilesMatrix.push_back(Matrix4x4(cam));
	BoundingBox temp;
	temp.maxPoint = Vector3D(NULL, NULL, NULL);
	temp.minPoint = Vector3D(NULL, NULL, NULL);
	projectilesBoundingBoxes.push_back(temp);
}

void GameWindow::onLeftMouseUp(const Point& mouse_position)
{

}

void GameWindow::onRightMouseDown(const Point& mouse_position)
{

}

void GameWindow::onRightMouseUp(const Point& mouse_position)
{

}
