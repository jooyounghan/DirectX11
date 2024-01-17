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
	cpTexture2D.Reset();
	cpFormatChangedUAV.Reset();
	cpFormatChangedTexture.Reset();

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

		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, uiWidth, uiHeight, 
			uiArraySize, uiNumQualityLevels,
			D3D11_BIND_UNORDERED_ACCESS, NULL, NULL, D3D11_USAGE_DEFAULT, 
			eFormat, cpFormatChangedTexture.GetAddressOf()
		);
		ID3D11Helper::CreateUnorderedAccessView(DirectXDevice::pDevice, cpFormatChangedTexture.Get(), cpFormatChangedUAV.GetAddressOf());
	}
	else
	{
		Console::Print("Swap Chain으로부터 Back Buffer를 가져올 수 없습니다.");
	}
}

void ASwapChainAccessable::ResetFromSwapChain()
{
	cpTexture2D.Reset();
	cpFormatChangedUAV.Reset();
	cpFormatChangedTexture.Reset();

	IsSwapChainAccesssed = false;
	uiWidth = NULL;
	uiHeight = NULL;
	uiArraySize = NULL;
	uiNumQualityLevels = NULL;
	eFormat = DXGI_FORMAT_UNKNOWN;
}