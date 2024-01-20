#include "ACamera.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"

ACamera::ACamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: ARenderTarget(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	ADepthStencil(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL, NULL, NULL,
		D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	ASwapChainAccessable(),
	AViewable(
		fXPos, fYPos, fZPos,
		(float)uiWidthIn, (float)uiHeightIn, fFovRadIn,
		fNearZIn, fFarZIn
	),
	ARectangle(
		uiWidthIn,
		uiHeightIn
	)
{
}

ACamera::~ACamera()
{

}

void ACamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (IsSwapChainAccesssed)
	{
		SetRectangle(uiWidthIn, uiHeightIn);

		ADepthStencil::Resize(uiWidthIn, uiHeightIn);
		ARenderTarget::Resize(uiWidthIn, uiHeightIn);

		// 백버퍼 및 Swap Chain 사이즈 변경
		ASwapChainAccessable::cpTexture2D.Reset();
		DirectXDevice::pSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		SetAsSwapChainBackBuffer();
	}
}
