#include "Material.h"
#include "GraphicsEngine.h"
#include <exception>

Material::Material(const wchar_t* VS_path, const wchar_t* PS_path)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(VS_path, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	//m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), list_size, shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	if (!m_vertex_shader) throw std::exception("Material creation error (vs)");

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(PS_path, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	if (!m_pixel_shader) throw std::exception("Material creation error (ps)");
}

Material::Material(const MTPtr& material)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

Material::~Material()
{
}

void Material::addTexture(const TPtr& texture)
{
	vec_textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->vec_textures.size()) return;
	vec_textures.erase(vec_textures.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	if (!constant_buffer)
		constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
	else
		constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
}

void Material::setCullMode(CULL_MODE mode)
{
	cull_mode = mode;
}

CULL_MODE Material::getCullMode()
{
	return cull_mode;
}
