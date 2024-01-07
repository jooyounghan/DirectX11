#include "ASwapChainAccessable.h"
#include "ID3D11Helper.h"

ASwapChainAccessable::ASwapChainAccessable(
	ID3D11Device* pDeviceIn,
	IDXGISwapChain* pSwapChainIn
) : pSwapChain(pSwapChainIn), IsSwapChainAccesssed(false), 
	ITexture2D(pDeviceIn, 1, 1, 1, 0, NULL, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM)
{

}

ASwapChainAccessable::~ASwapChainAccessable()
{
}

void ASwapChainAccessable::SetAsSwapChainBackBuffer()
{
	ResetFromSwapChain();
	IsSwapChainAccesssed = true;

	if (ID3D11Helper::GetBackBuffer(pSwapChain, cpTexture2D.GetAddressOf()))
	{
		D3D11_TEXTURE2D_DESC sTexture2DDesc;
		cpTexture2D->GetDesc(&sTexture2DDesc);
		uiWidth = sTexture2DDesc.Width;
		uiHeight = sTexture2DDesc.Height;
		uiArraySize = sTexture2DDesc.ArraySize;
		uiNumQualityLevels = sTexture2DDesc.SampleDesc.Quality;
		eFormat = sTexture2DDesc.Format;
	}
}

void ASwapChainAccessable::ResetFromSwapChain()
{
	IsSwapChainAccesssed = false;
	cpTexture2D.Reset();
}