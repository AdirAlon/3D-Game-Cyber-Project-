#pragma once
#include <d3d11.h>
#include "prereq.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT buffer_size, RenderSystem* system);
	void update(DCPtr context, void* buffer);
	~ConstantBuffer();
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

