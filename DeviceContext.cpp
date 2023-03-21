#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system):m_system(system), m_device_context(device_context)
{

}

void DeviceContext::clearRenderTargetColor(const SCPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = {red,green,blue,alpha};
	m_device_context->ClearRenderTargetView(swap_chain->m_render_target_view, clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_render_target_view, swap_chain->m_dv);
}

void DeviceContext::setVertexBuffer(const VBPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_vertex_size;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);

	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT vertex_index_start)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, vertex_index_start);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT vertex_index_start)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, vertex_index_start);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::setVertexShader(const VSPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vertex_shader, nullptr, 0);
}

void DeviceContext::setPixelShader(const PSPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_pixel_shader, nullptr, 0);
}

void DeviceContext::setConstantBuffer(const VSPtr& vertex_shader, const CBPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PSPtr& pixel_shader, const CBPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setTexture(const VSPtr& vertex_shader, const TPtr* texture, unsigned int tex_num)
{
	ID3D11ShaderResourceView* res_list[32];
	ID3D11SamplerState* res_samp[32];
	for (unsigned int i = 0; i < tex_num; i++)
	{
		res_list[i] = texture[i]->m_shader_resV;
		res_samp[i] = texture[i]->m_sampler_state;
	}
	m_device_context->VSSetShaderResources(0, tex_num, res_list);
	m_device_context->VSSetSamplers(0, tex_num, res_samp);
}

void DeviceContext::setTexture(const PSPtr& pixel_shader, const TPtr* texture, unsigned int tex_num)
{
	ID3D11ShaderResourceView* res_list[32];
	ID3D11SamplerState* res_samp[32];
	for (unsigned int i = 0; i < tex_num; i++)
	{
		res_list[i] = texture[i]->m_shader_resV;
		res_samp[i] = texture[i]->m_sampler_state;
	}
	m_device_context->PSSetShaderResources(0, tex_num, res_list);
	m_device_context->PSSetSamplers(0, tex_num, res_samp);
}

void DeviceContext::setIndexBuffer(const IBPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location,  start_vertex_index);
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}
