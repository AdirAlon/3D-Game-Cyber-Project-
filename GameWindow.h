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
	void drawMesh(const MPtr& mesh, const VSPtr& vertex_shader, const PSPtr& pixel_shader, const CBPtr& constant_buffer, const TPtr* texture_list, unsigned int tex_num);
	void updateCamera();
	void updateModel();
	void updateSky();

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
	TPtr m_brick_tex;
	TPtr m_sky_tex;
	TPtr m_specular_tex;
	MPtr m_mesh;
	MPtr m_sky_sphere;

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
	float m_forward = 0.0f;
	float m_sideward = 0.0f;
	float speed = 0.2f;
	float jump = 0.0f;
	bool game_state = false;
	bool fullscreen = false;
	float m_light_radius = 4.0f;


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

