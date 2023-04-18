#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VMesh.h"
#include <vector>

struct MaterialPlace
{
	size_t start = 0;
	size_t num = 0;
	size_t id = 0;
};
class Mesh: public Resource
{
public:
	Mesh(const wchar_t* path);
	~Mesh();
	const VBPtr& getVertexBuffer();
	const IBPtr& getIndexBuffer();
	const MaterialPlace& getMaterialPlace(unsigned int place);
	size_t getMaterialPlaceNum();
	std::vector<VMesh> v_list;
private:
	VBPtr m_vb;
	IBPtr m_ib;
	std::vector<MaterialPlace> m_materialPlaces;
private:
	friend class DeviceContext;
};

