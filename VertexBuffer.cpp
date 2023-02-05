#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>


VertexBuffer::VertexBuffer(void* vertices_list, UINT vertex_size, UINT list_size, void* byte_code_shader, UINT byte_shader_size, RenderSystem* system) : m_system(system), m_layout(0), m_buffer(0)
{
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = vertex_size * list_size;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = vertices_list;

    m_vertex_size = vertex_size;
    m_list_size = list_size;

    if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
    {
        throw std::exception("VertexBuffer Creation Failed (1)");
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT layout_size = ARRAYSIZE(layout);

    if (FAILED(m_system->m_d3d_device->CreateInputLayout(layout, layout_size, byte_code_shader, byte_shader_size, &m_layout)))
    {
        throw std::exception("InputLayout Creation Failed (2)");
    }
}

UINT VertexBuffer::getSizeVertexList()
{
    return this->m_list_size;
}

VertexBuffer::~VertexBuffer()
{
    m_layout->Release();
    m_buffer->Release();
}
