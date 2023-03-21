#pragma once
#include <d3d11.h>
#include "prereq.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	bool present(bool vsync);
	void resize(unsigned int width, unsigned int height);
	void setFullScreen(bool fullscreen, unsigned int width, unsigned int height);
	~SwapChain();
private:
	void reloadBuffers(unsigned int width, unsigned int height);
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;
	RenderSystem* m_system = nullptr;
	ID3D11DepthStencilView* m_dv = nullptr;
private:
	friend class DeviceContext;
};

