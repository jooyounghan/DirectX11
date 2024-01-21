#include "ASwapChainAccessable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ASwapChainAccessable::ASwapChainAccessable() 
	: IsSwapChainAccesssed(false), Texture2D()
{

}

ASwapChainAccessable::~ASwapChainAccessable()
{
}

void ASwapChainAccessable::SetAsSwapChainBackBuffer()
{
	cpTexture2D.Reset();
	IsSwapChainAccesssed = true;

	if (ID3D11Helper::GetBackBuffer(DirectXDevice::pSwapChain, cpTexture2D.GetAddressOf()))
	{
		D3D11_TEXTURE2D_DESC sDesc;
		cpTexture2D->GetDesc(&sDesc);
		uiWidth = sDesc.Width;
		uiHeight = sDesc.Height;
		uiArraySize = sDesc.ArraySize;
		uiNumQualityLevels = sDesc.SampleDesc.Quality;
		eFormat = sDesc.Format;
	}
	else
	{
		Console::AssertPrint("Swap Chain으로부터 Back Buffer를 가져올 수 없습니다.");
	}
}

void ASwapChainAccessable::ResetFromSwapChain()
{
	cpTexture2D.Reset();
	IsSwapChainAccesssed = false;
	uiWidth = NULL;
	uiHeight = NULL;
	uiArraySize = NULL;
	uiNumQualityLevels = NULL;
	eFormat = DXGI_FORMAT_UNKNOWN;
}