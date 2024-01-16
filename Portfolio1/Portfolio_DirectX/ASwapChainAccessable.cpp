#include "ASwapChainAccessable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ASwapChainAccessable::ASwapChainAccessable() 
	: IsSwapChainAccesssed(false), ITexture2D()
{

}

ASwapChainAccessable::~ASwapChainAccessable()
{
}

void ASwapChainAccessable::SetAsSwapChainBackBuffer()
{
	ResetFromSwapChain();
	IsSwapChainAccesssed = true;

	if (ID3D11Helper::GetBackBuffer(DirectXDevice::pSwapChain, cpTexture2D.GetAddressOf()))
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