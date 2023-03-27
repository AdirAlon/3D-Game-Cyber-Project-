#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>
#include <codecvt>
#include "GraphicsEngine.h"
#include "VMesh.h"

Mesh::Mesh(const wchar_t* path): Resource(path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	std::string warnings;
	std::string errors;
	//std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(path);
	std::wstring ws(path);
	std::string inputfile(ws.begin(), ws.end());
	//int size_needed = WideCharToMultiByte(CP_UTF8, 0, &path[0], wcslen(path), NULL, 0, NULL, NULL);
	//std::string inputfile(size_needed, 0);
	//WideCharToMultiByte(CP_UTF8, 0, &path[0], wcslen(path), &inputfile[0], size_needed, NULL, NULL);
	//WideCharToMultiByte(CP_UTF8, 0, &path[0], -1, &inputfile[0], size_needed, NULL, NULL);
	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));
	bool res = tinyobj::LoadObj(&attrib, &shapes, &mats, &warnings, &errors, inputfile.c_str(), mtldir.c_str());

	if(!errors.empty())
		throw std::exception("Mesh creation Failed");
	if (!res)
		throw std::exception("Mesh creation Failed");

	std::vector<VMesh> v_list;
	std::vector<unsigned int> i_list;
	size_t lists_size = 0;
	for (size_t i = 0; i < shapes.size(); i++)
	{
		lists_size += shapes[i].mesh.indices.size();

	}

	v_list.reserve(lists_size);
	i_list.reserve(lists_size);
	m_materialPlaces.resize(mats.size());
	size_t index_offset = 0;
	for (size_t n = 0; n < mats.size(); n++)
	{
		m_materialPlaces[n].start = index_offset;
		m_materialPlaces[n].id = n;
		for (size_t i = 0; i < shapes.size(); i++)
		{
			size_t offset = 0;
			v_list.reserve(shapes[i].mesh.indices.size());
			i_list.reserve(shapes[i].mesh.indices.size());
			for (size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++)
			{
				if (shapes[i].mesh.material_ids[j] != n) continue;

				unsigned char FVN = shapes[i].mesh.num_face_vertices[j];

				for (unsigned char g = 0; g < FVN; g++)
				{
					tinyobj::index_t index = shapes[i].mesh.indices[offset + g];
					tinyobj::real_t vx = attrib.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attrib.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attrib.vertices[index.vertex_index * 3 + 2];
					tinyobj::real_t tx = attrib.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attrib.texcoords[index.texcoord_index * 2 + 1];
					tinyobj::real_t nx = attrib.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attrib.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attrib.normals[index.normal_index * 3 + 2];
					VMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
					v_list.push_back(vertex);
					i_list.push_back((unsigned int)index_offset + g);
				}
				offset += FVN;
				index_offset += FVN;
			}
		}
		m_materialPlaces[n].num = index_offset - m_materialPlaces[n].start;
	}
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshShaderBC(&shader_byte_code, &size_shader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&v_list[0], sizeof(VMesh), (UINT)v_list.size(), shader_byte_code, (UINT)size_shader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&i_list[0], (UINT)i_list.size());
}

Mesh::~Mesh()
{
}

const VBPtr& Mesh::getVertexBuffer()
{
	return m_vb;
}

const IBPtr& Mesh::getIndexBuffer()
{
	return m_ib;
}

const MaterialPlace& Mesh::getMaterialPlace(unsigned int place)
{
	if (place >= m_materialPlaces.size()) return MaterialPlace();
	return m_materialPlaces[place];
}

size_t Mesh::getMaterialPlaceNum()
{
	return m_materialPlaces.size();
}
