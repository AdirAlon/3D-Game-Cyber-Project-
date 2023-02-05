#pragma once
#include <d3d11.h>
#include "prereq.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	void clearRenderTargetColor(const SCPtr& swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(const VBPtr& vertex_buffer);
	void drawTriangleList(UINT vertex_count, UINT vertex_index_start);
	void drawTriangleStrip(UINT vertex_count, UINT vertex_index_start);
	void setViewportSize(UINT width, UINT height);
	void setVertexShader(const VSPtr& vertex_shader);
	void setPixelShader(const PSPtr& pixel_shader);
	void setConstantBuffer(const VSPtr& vertex_shader, const CBPtr& buffer);
	void setConstantBuffer(const PSPtr& pixel_shader, const CBPtr& buffer);
	void setTexture(const VSPtr& vertex_shader, const TPtr& texture);
	void setTexture(const PSPtr& pixel_shader, const TPtr& texture);
	void setIndexBuffer(const IBPtr& index_buffer);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_system = nullptr;
private:
	friend class ConstantBuffer;
};

