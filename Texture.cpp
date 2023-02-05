#include "Texture.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* path): Resource(path)
{
	DirectX::ScratchImage imageD;
	HRESULT res = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, imageD);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, imageD.GetImages(), imageD.GetImageCount(), imageD.GetMetadata(), &m_texture);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageD.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageD.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;
		GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shader_resV);


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
