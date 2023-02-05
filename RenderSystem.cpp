#include "RenderSystem.h"
#include <d3d11.h>
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include <d3dcompiler.h>
#include <exception>

ID3D11DeviceContext* m_imm_context;


RenderSystem::RenderSystem()
{
    D3D_DRIVER_TYPE driver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT driverTypeNum = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT featureLevelNum = ARRAYSIZE(featureLevels);

    //ID3D11DeviceContext* m_imm_context;
    HRESULT res = 0;
    for (UINT i = 0; i < driverTypeNum;)
    {
        res = D3D11CreateDevice(NULL, driver_types[i], NULL, NULL, featureLevels, featureLevelNum, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

        if (SUCCEEDED(res))
            break;
        ++i;
    }

    if (FAILED(res))
    {
        throw std::exception("RenderSystem Creation Failed");
    }

    m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context, this);


    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
}

SCPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
    SCPtr sc = nullptr;
    try
    {
        sc = std::make_shared<SwapChain>(hwnd, width, height, this);
    }
    catch(...){}
    return sc;
}

DCPtr RenderSystem::getImmediateDeviceContext()
{
    return this->m_imm_device_context;
}

VBPtr RenderSystem::createVertexBuffer(void* vertices_list, UINT vertex_size, UINT list_size, void* byte_code_shader, UINT byte_shader_size)
{
    VBPtr vb = nullptr;
    try
    {
        vb = std::make_shared<VertexBuffer>(vertices_list, vertex_size, list_size, byte_code_shader, byte_shader_size, this);
    }
    catch (...) {}
    return vb;
}

IBPtr RenderSystem::createIndexBuffer(void* indices_list, UINT list_size)
{
    IBPtr ib = nullptr;
    try
    {
        ib = std::make_shared<IndexBuffer>(indices_list, list_size, this);
    }
    catch (...) {}
    return ib;
}

VSPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
    VSPtr vs = nullptr;
    try
    {
        vs = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
    }
    catch (...) {}
    return vs;
}

PSPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
    PSPtr ps = nullptr;
    try
    {
        ps = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
    }
    catch (...) {}
    return ps;
}

CBPtr RenderSystem::createConstantBuffer(void* buffer, UINT buffer_size)
{
    CBPtr cb = nullptr;
    try
    {
        cb = std::make_shared<ConstantBuffer>(buffer, buffer_size, this);
    }
    catch (...) {}
    return cb;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
    ID3DBlob* error_blob = nullptr;
    if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
    {
        if (error_blob) error_blob->Release();
        return false;
    }

    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();

    return true;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
    ID3DBlob* error_blob = nullptr;
    if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
    {
        if (error_blob) error_blob->Release();
        return false;
    }

    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();

    return true;
}

void RenderSystem::releaseCompiledShader()
{
    if (m_blob)m_blob->Release();
}

RenderSystem::~RenderSystem()
{
    if (m_vsblob)m_vsblob->Release();
    if (m_psblob)m_psblob->Release();

    m_dxgi_device->Release();
    m_dxgi_adapter->Release();
    m_dxgi_factory->Release();

    m_d3d_device->Release();
}
