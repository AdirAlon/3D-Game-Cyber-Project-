#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void* buffer, UINT buffer_size, RenderSystem* system): m_system(system)
{ 

        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.ByteWidth = buffer_size;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA init_data = {};
        init_data.pSysMem = buffer;

        if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
        {
            throw std::exception("ConstantBuffer Creation Failed");
        }
}

void ConstantBuffer::update(DCPtr context, void* buffer)
{
    context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);

}

ConstantBuffer::~ConstantBuffer()
{
    if (m_buffer)m_buffer->Release();
}