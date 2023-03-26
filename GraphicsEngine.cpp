#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>
#include "DeviceContext.h"
GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
    try
    {
        m_render_system = new RenderSystem();
    }
    catch (...) { throw std::exception("RenderSystem Creation Failed"); }

    try
    {
        m_tex_man = new TextureManager();
    }
    catch (...) { throw std::exception("TextureManager Creation Failed"); }

    try
    {
        m_mm = new MeshManager();
    }
    catch (...) { throw std::exception("MeshManager Creation Failed"); }

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;
    m_render_system->compileVertexShader(L"VertexMeshShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
    ::memcpy(m_mesh_bc, shader_byte_code, size_shader);
    m_mesh_size = size_shader;
    m_render_system->releaseCompiledShader();
}

TextureManager* GraphicsEngine::getTextureManager()
{
    return m_tex_man;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
    return m_render_system;
}

MeshManager* GraphicsEngine::getMeshManager()
{
    return m_mm;
}

void GraphicsEngine::getVertexMeshShaderBC(void** byte_code, size_t* size)
{
    *byte_code = m_mesh_bc;
    *size = m_mesh_size;
}

MTPtr GraphicsEngine::createMaterial(const wchar_t* VS_path, const wchar_t* PS_path)
{
    MTPtr MT = nullptr;
    try
    {
        MT = std::make_shared<Material>(VS_path, PS_path);
    }
    catch (...) {}
    return MT;
}

MTPtr GraphicsEngine::createMaterial(const MTPtr& material)
{
    MTPtr MT = nullptr;
    try
    {
        MT = std::make_shared<Material>(material);
    }
    catch (...) {}
    return MT;
}

void GraphicsEngine::setMaterial(const MTPtr& material)
{
    GraphicsEngine::get()->getRenderSystem()->setRasterizerState(material->cull_mode == CULL_MODE_FRONT);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_vertex_shader, material->constant_buffer);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_pixel_shader, material->constant_buffer);

    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->m_vertex_shader);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->m_pixel_shader);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(material->m_pixel_shader, &material->vec_textures[0], material->vec_textures.size());
}

void GraphicsEngine::create()
{
    if (GraphicsEngine::m_engine)
        throw std::exception("GraphicsEngine Exist");
    GraphicsEngine::m_engine = new GraphicsEngine();
}

GraphicsEngine* GraphicsEngine::get()
{
    return m_engine;
}

void GraphicsEngine::release()
{
    if (!GraphicsEngine::m_engine)
        return;
    delete GraphicsEngine::m_engine;
}

GraphicsEngine::~GraphicsEngine()
{
    GraphicsEngine::m_engine = nullptr;
    delete m_tex_man;
    delete m_render_system;
    delete m_mm;
}
