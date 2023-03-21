#include "Texture.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* path): Resource(path)
{
	DirectX::ScratchImage imageD;
	HRESULT res = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageD);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, imageD.GetImages(), imageD.GetImageCount(), imageD.GetMetadata(), &m_texture);
		if (FAILED(res)) throw std::exception("Texture creation Failed");
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageD.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageD.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC smapler_desc = {};
		smapler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		smapler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		smapler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		smapler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		smapler_desc.MinLOD = 0;
		smapler_desc.MaxLOD = (UINT)imageD.GetMetadata().mipLevels;

		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&smapler_desc, &m_sampler_state);
		if (FAILED(res)) throw std::exception("Texture creation Failed");
		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shader_resV);
		if (FAILED(res)) throw std::exception("Texture creation Failed");


	}
	else
	{
		throw std::exception("Texture creation Failed");
	}
}

Texture::~Texture()
{
	m_shader_resV->Release();
	m_texture->Release();
}
