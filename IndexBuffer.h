#pragma once
#include <d3d11.h>
#include "prereq.h"

class IndexBuffer
{
public:
	IndexBuffer(void* indices_list, UINT list_size, RenderSystem * system);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_list_size;
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

