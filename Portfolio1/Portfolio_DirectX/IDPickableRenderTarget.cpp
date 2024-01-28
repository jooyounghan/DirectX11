#include "IDPickableRenderTarget.h"
#include "Shaders.h"
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
	256, 1, 1),
	RenderTarget(
	uiWidthIn, uiHeightIn,
	1, uiNumQualityLevelsIn, D3D11_BIND_RENDER_TARGET,
	NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
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

	sMousePosNdc.uiMouseXNdc = (uint32_t)iMouseX;
	sMousePosNdc.uiMouseYNdc = (uint32_t)iMouseY;
}

void IDPickableRenderTarget::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	Shaders& shaders = Shaders::GetInstance();

	DirectXDevice::pDeviceContext->CSSetShader(shaders.GetComputeShader(Shaders::ResolveCS), NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppInputSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, IDPickableRenderTarget::cpUAV.GetAddressOf(), nullptr);
	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ
	);
	SetUAVBarrier();
}

void IDPickableRenderTarget::SetUAVBarrier()
{
	ID3D11ShaderResourceView* pReleaseAndGetAddressOfSRV = nullptr;
	ID3D11UnorderedAccessView* pReleaseAndGetAddressOfUAV = nullptr;

	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pReleaseAndGetAddressOfSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pReleaseAndGetAddressOfUAV, nullptr);
}


void IDPickableRenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	AFilter::Resize(uiWidthIn, uiHeightIn);
	RenderTarget::Resize(uiWidthIn, uiHeightIn);
}
