#pragma once
#include <d3d11.h>
#include "prereq.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	bool present(bool vsync);
	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;
	RenderSystem* m_system = nullptr;
	ID3D11DepthStencilView* m_dv = nullptr;
private:
	friend class DeviceContext;
};

