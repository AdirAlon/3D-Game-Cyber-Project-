#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* indices_list, UINT list_size, RenderSystem* system) : m_system(system), m_buffer(0)
{

    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = 4 * list_size;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = indices_list;

    m_list_size = list_size;

    if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
    {
        throw std::exception("IndexBuffer Creation Failed");
    }
}


UINT IndexBuffer::getSizeIndexList()
{
    return this->m_list_size;
}

IndexBuffer::~IndexBuffer()
{
    m_buffer->Release();
}

