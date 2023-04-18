#pragma once
#include <d3d11.h>
#include "prereq.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
	void setRasterizerState(bool front_cull);

public:
	SCPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DCPtr getImmediateDeviceContext();
	VBPtr createVertexBuffer(void* vertices_list, UINT vertex_size, UINT list_size, void* byte_code_shader, UINT byte_shader_size);
	IBPtr createIndexBuffer(void* indices_list, UINT list_size);
	VSPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PSPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	CBPtr createConstantBuffer(void* buffer, UINT buffer_size);
private:
	void RasterizerState();

private:
	DCPtr m_imm_device_context;

public:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

private:
	ID3DBlob* m_blob = nullptr;
	ID3D11RasterizerState* m_cull_front = nullptr;
	ID3D11RasterizerState* m_cull_back = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class Texture;
};