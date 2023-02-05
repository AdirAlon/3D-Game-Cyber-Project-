#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh: public Resource
{
public:
	Mesh(const wchar_t* path);
	~Mesh();
	const VBPtr& getVertexBuffer();
	const IBPtr& getIndexBuffer();
private:
	VBPtr m_vb;
	IBPtr m_ib;
private:
	friend class DeviceContext;
};

