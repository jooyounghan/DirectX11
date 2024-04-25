#include "IDPickableRenderTarget.h"

#include "ModelIDResolveComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include <algorithm>

IDPickableRenderTarget::IDPickableRenderTarget(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiNumQualityLevelsIn
) : AFilter(
	uiWidthIn, uiHeightIn,
	1, 0, D3D11_BIND_UNORDERED_ACCESS,
	NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
	32, 32, 1),
	RenderTarget(
	uiWidthIn, uiHeightIn,
	1, uiNumQualityLevelsIn, D3D11_BIND_RENDER_TARGET,
	NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	pModelIDResolveCS = ModelIDResolveComputeShader::GetInstance();

	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, 1, 1,
		1, 0, NULL,
		D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R32_UINT,
		cpPickedIDResult.GetAddressOf()
	);
}

IDPickableRenderTarget::~IDPickableRenderTarget()
{
}

void IDPickableRenderTarget::SetMousePos(const int& iMouseXIn, const int& iMouseYIn)
{
	const int iMouseX = std::clamp((int)iMouseXIn, 0, (int)uiWidth - 1);
	const int iMouseY = std::clamp((int)iMouseYIn, 0, (int)uiHeight - 1);

	sMousePosPixel.uiMouseXPixel = (uint32_t)iMouseX;
	sMousePosPixel.uiMouseYPixel = (uint32_t)iMouseY;
}

void IDPickableRenderTarget::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	pModelIDResolveCS->ApplyShader();
	pModelIDResolveCS->SetShader(ppInputSRV, IDPickableRenderTarget::cpUAV.GetAddressOf());
	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ
	);
	pModelIDResolveCS->ResetShader();
	pModelIDResolveCS->DisapplyShader();
}

void IDPickableRenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	AFilter::Resize(uiWidthIn, uiHeightIn);
	RenderTarget::Resize(uiWidthIn, uiHeightIn);
}
