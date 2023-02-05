#pragma once
#include <d3d11.h>
#include "prereq.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
public:
	TextureManager* getTextureManager();
	RenderSystem* getRenderSystem();
	MeshManager* getMeshManager();
	void getVertexMeshShaderBC(void** byte_code, size_t* size);
public:
	static GraphicsEngine* get();
	static void release();
	static void create();
private:
	TextureManager* m_tex_man = nullptr;
	RenderSystem* m_render_system = nullptr;
	static GraphicsEngine* m_engine;
	unsigned char m_mesh_bc[1024];
	size_t m_mesh_size = 0;
	MeshManager* m_mm = nullptr;

};

