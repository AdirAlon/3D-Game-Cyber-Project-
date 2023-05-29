#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class GameWindow: public Window, public InputListener
{
public:
	GameWindow();
	~GameWindow();
	void update();
	void render();
	void drawMesh(const MPtr& mesh, const std::vector<MTPtr>& material_list);
	void updateCamera();
	void updateModel(MPtr mesh, Vector3D pos, const std::vector<MTPtr>& material_list);
	void updateSky();
	void updateBullet();
	void updateLight();
	void setEnemyPos(int x, int y, int z);
	Vector4D getDirections();
	Vector3D new_pos;
	Vector3D playerMin;
	Vector3D playerMax;
	Vector3D projMin;
	Vector3D projMax;
	Matrix4x4 m_light_rot;
	Matrix4x4 transMat;
	Matrix4x4 world;
	bool hit = false;
	float m_forward = 0.0f;
	float m_sideward = 0.0f;
	float m_upward = 0.0f;
	float speed = 0.2f;

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;

private:
	SCPtr m_swap_chain;
	VBPtr m_vertex_buffer;
	VSPtr m_vertex_shader;
	PSPtr m_pixel_shader;
	PSPtr m_sky_pixel_shader;
	CBPtr m_constant_buffer; 
	CBPtr m_sky_constant_buffer;
	IBPtr m_index_buffer;
	TPtr m_cobble_tex;
	TPtr m_bricks_tex;
	TPtr m_wall_tex;
	TPtr m_earth_tex;
	TPtr m_sky_tex;
	TPtr m_specular_tex;
	TPtr m_plane_tex;
	TPtr m_barrel_tex;
	TPtr m_brick_tex;
	TPtr m_windows_tex;
	TPtr m_wood_tex;
	TPtr m_box_tex;
	MPtr m_mesh;
	MPtr m_plane_mesh;
	MPtr m_sky_sphere;
	MPtr m_house_mesh;
	MPtr m_box_mesh;
	MPtr m_bullet_mesh;
	MTPtr m_material;
	MTPtr m_box_material;
	MTPtr m_sky_material;
	MTPtr m_plane_material;
	MTPtr m_barrel_material;
	MTPtr m_brick_material;
	MTPtr m_windows_material;
	MTPtr m_wood_material;
	


private:
	float m_old_delta;
	float m_new_delta;
	float m_delta_time;
	float m_delta_position;
	float m_delta_scale;
	float m_rotaion_x = 0.0f;
	float m_rotaion_y = 0.0f;
	float m_light_rotaion_y = 0.0f;
	float m_cube_scale = 1;
	Matrix4x4 m_world_camera;
	Matrix4x4 m_view_camera;
	Matrix4x4 m_projection_camera;
	float jump = 0.0f;
	bool game_state = false;
	bool fullscreen = false;
	float m_light_radius = 1000.0f;
	Vector4D m_light_pos;
	std::vector<MTPtr> m_material_list;
	float larlar = 0.0f;


public:
	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_position) override;
	virtual void onLeftMouseDown(const Point& mouse_position) override;
	virtual void onLeftMouseUp(const Point& mouse_position) override;
	virtual void onRightMouseDown(const Point& mouse_position) override;
	virtual void onRightMouseUp(const Point& mouse_position) override;
};

