#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include <vector>
#include "ConstantBuffer.h"

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};
class GraphicsEngine;
class Material
{
public:
	Material(const wchar_t* VS_path, const wchar_t* PS_path);
	Material(const MTPtr& material);
	~Material();
	void addTexture(const TPtr& texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size);
	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();

private:
	VSPtr m_vertex_shader;
	PSPtr m_pixel_shader;
	std::vector<TPtr> vec_textures;
	CBPtr constant_buffer;
	CULL_MODE cull_mode = CULL_MODE_BACK;
	friend class GraphicsEngine;
};

