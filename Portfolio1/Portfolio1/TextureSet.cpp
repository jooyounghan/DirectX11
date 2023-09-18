#include "TextureSet.h"
#include "ID3D11Helper.h"

void TextureSet::CreateTextureAndSRV(
	IN TextureType eTextureType,
	IN ID3D11Device* pDevice,
	IN ID3D11DeviceContext* pDeviceContext,
	IN ImageContainer* pImageContainer
)
{
	ID3D11Texture2D** ppTexture2D = nullptr;
	ID3D11ShaderResourceView** ppSRV = nullptr;
	switch (eTextureType)
	{
	case TEXTURE_AO:
		ppTexture2D = AOTexture2D.GetAddressOf();
		ppSRV = AOSRV.GetAddressOf();
		break;
	case TEXTURE_DIFFUSE:
		ppTexture2D = DiffuseTexture2D.GetAddressOf();
		ppSRV = DiffuseSRV.GetAddressOf();
		break;
	case TEXTURE_HEIGHT:
		ppTexture2D = HeightTexture2D.GetAddressOf();
		ppSRV = HeightSRV.GetAddressOf();
		break;
	case TEXTURE_NORMAL:
		ppTexture2D = NormalTexture2D.GetAddressOf();
		ppSRV = NormalSRV.GetAddressOf();
		break;
	case TEXTURE_REFLECT:
		ppTexture2D = ReflectTexture2D.GetAddressOf();
		ppSRV = ReflectSRV.GetAddressOf();
		break;
	}
	ID3D11Helper::CreateTexture2D(pDevice, pDeviceContext, pImageContainer, ppTexture2D);
	ID3D11Helper::CreateShaderResoureView(pDevice, *ppTexture2D, ppSRV);
	pDeviceContext->GenerateMips(*ppSRV);
}