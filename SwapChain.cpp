#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : m_system(system)
{
    ID3D11Device* device = m_system->m_d3d_device;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.Windowed = TRUE;

    //Create the window's swap chain
    HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
    if (FAILED(hr))
    {
        throw std::exception("SwapChain Creation Failed (1)");
    }
    reloadBuffers(width, height);
}

bool SwapChain::present(bool vsync)
{
    m_swap_chain->Present(vsync, NULL);

    return true;
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
    if (m_render_target_view) m_render_target_view->Release();
    if (m_dv) m_dv->Release();
    m_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    reloadBuffers(width, height);
}

void SwapChain::setFullScreen(bool fullscreen, unsigned int width, unsigned int height)
{
    resize(width, height);
    m_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

SwapChain::~SwapChain()
{
    m_dv->Release();
    m_render_target_view->Release();
    m_swap_chain->Release();
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
    ID3D11Device* device = m_system->m_d3d_device;
    ID3D11Texture2D* buffer = NULL;
    HRESULT hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

    if (FAILED(hr))
    {
        throw std::exception("SwapChain Creation Failed (2)");
    }

    hr = device->CreateRenderTargetView(buffer, NULL, &m_render_target_view);
    buffer->Release();

    if (FAILED(hr))
    {
        throw std::exception("SwapChain Creation Failed (3)");
    }

    D3D11_TEXTURE2D_DESC tex_d = {};
    tex_d.Width = width;
    tex_d.Height = height;
    tex_d.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex_d.Usage = D3D11_USAGE_DEFAULT;
    tex_d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex_d.MipLevels = 1;
    tex_d.SampleDesc.Count = 1;
    tex_d.SampleDesc.Quality = 0;
    tex_d.MiscFlags = 0;
    tex_d.ArraySize = 1;
    tex_d.CPUAccessFlags = 0;

    hr = device->CreateTexture2D(&tex_d, nullptr, &buffer);
    if (FAILED(hr))
    {
        throw std::exception("SwapChain Creation Failed (3)");
    }

    hr = device->CreateDepthStencilView(buffer, NULL, &m_dv);
    buffer->Release();

    if (FAILED(hr))
    {
        throw std::exception("SwapChain Creation Failed (3)");
    }
}
