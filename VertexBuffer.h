#pragma once
#include <d3d11.h>
#include "prereq.h"

class VertexBuffer
{
public:
	VertexBuffer(void* vertices_list, UINT vertex_size, UINT list_size, void* byte_code_shader, UINT byte_shader_size, RenderSystem* system);
	UINT getSizeVertexList();
	~VertexBuffer();
private:
	UINT m_vertex_size;
	UINT m_list_size;
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

