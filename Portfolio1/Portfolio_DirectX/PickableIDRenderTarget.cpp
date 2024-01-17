#include "PickableIDRenderTarget.h"
#include "StageShaderResource.h"

#include "DefineVar.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include <algorithm>

using namespace std;

PickableIDRenderTarget::PickableIDRenderTarget(
	const UINT& uiNumQualityLevelsIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: IRenderTarget(
		uiWidthIn, uiHeightIn, 
		1, uiNumQualityLevelsIn, NULL, NULL, NULL,
		D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_R16_UINT
	),
	IRectangle(
		uiWidthIn, uiHeightIn
	),
	pMSToSSSRV(nullptr), pStageSRV(nullptr)
{
	pStageSRV = new StageShaderResource(
		1, 1,
		1, 0, 
		DXGI_FORMAT_R16_UINT
	);

	if (uiNumQualityLevelsIn > 0)
	{
		pMSToSSSRV = new StageShaderResource(
			uiWidth, uiHeight,
			1, uiNumQualityLevels, DXGI_FORMAT_R16_UINT
		);
	}
}

PickableIDRenderTarget::~PickableIDRenderTarget()
{
	AutoDelete(pStageSRV);
	AutoDelete(pMSToSSSRV);
}

WORD PickableIDRenderTarget::GetPickedID(const UINT& uiMouseXIn, const UINT& uiMouseYIn)
{
	WORD wResult;

	D3D11_BOX sBox;
	AutoZeroMemory(sBox);
	sBox.left = std::clamp(uiMouseXIn, 0u, uiWidth);
	sBox.right = sBox.left + 1;
	sBox.top = std::clamp(uiMouseYIn, 0u, uiHeight);
	sBox.bottom = sBox.top + 1;
	sBox.front = 0;
	sBox.back = 1;

	ID3D11Texture2D* pSourceTexture = nullptr;

	if (uiNumQualityLevels > 0)
	{
		pDeviceContext->ResolveSubresource(pMSToSSSRV->cpTexture2D.Get(), 0, cpTexture2D.Get(), 0, eFormat);
		pSourceTexture = pMSToSSSRV->cpTexture2D.Get();
	}
	else
	{
		pSourceTexture = cpTexture2D.Get();
	}
	pDeviceContext->CopySubresourceRegion(pStageSRV->cpTexture2D.Get(), 0, 0, 0, 0, pSourceTexture, 0, &sBox);

	D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
	AutoZeroMemory(sMappedSubResource);
	pDeviceContext->Map(pStageSRV->cpTexture2D.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
	memcpy(&wResult, sMappedSubResource.pData, sizeof(WORD));
	pDeviceContext->Unmap(pStageSRV->cpTexture2D.Get(), 0);

	return wResult;
}

void PickableIDRenderTarget::ClearRTV()
{
	pDeviceContext->ClearRenderTargetView(cpRTV.Get(), IRenderTarget::fClearColor);
}

void PickableIDRenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpRTV.Reset();
	cpTexture2D.Reset();
	pMSToSSSRV->cpSRV.Reset();
	pMSToSSSRV->cpTexture2D.Reset();

	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;

	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, uiWidth, uiHeight,
		uiArraySize, uiNumQualityLevels,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT, eFormat, cpTexture2D.GetAddressOf()
	);
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, cpTexture2D.Get(), cpRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());

	if (uiNumQualityLevels > 0)
	{
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, uiWidth, uiHeight,
			uiArraySize, 0,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			NULL, NULL, D3D11_USAGE_DEFAULT, eFormat, pMSToSSSRV->cpTexture2D.GetAddressOf()
		);
		ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, pMSToSSSRV->cpTexture2D.Get(), pMSToSSSRV->cpSRV.GetAddressOf());
	}
}
