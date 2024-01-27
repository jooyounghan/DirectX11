#include "AFilter.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

AFilter::AFilter(
	UINT uiThreadGroupCntXIn,
	UINT uiThreadGroupCntYIn,
	UINT uiThreadGroupCntZIn
) : ShaderResource(
	1, 1, 1, 0, 
	NULL, NULL, NULL, 
	D3D11_USAGE::D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM
	),
	uiThreadGroupCntX(uiThreadGroupCntXIn),
	uiThreadGroupCntY(uiThreadGroupCntYIn),
	uiThreadGroupCntZ(uiThreadGroupCntZIn)
{
}

AFilter::AFilter(
	UINT uiWidthIn, 
	UINT uiHeightIn, 
	UINT uiArraySizeIn, 
	UINT uiNumQualityLevelsIn, 
	UINT uiBindFlagIn, 
	UINT uiCPUAccessIn, 
	UINT uiMiscFlagIn, 
	D3D11_USAGE eUsageIn, 
	DXGI_FORMAT eFormatIn,
	UINT uiThreadGroupCntXIn,
	UINT uiThreadGroupCntYIn,
	UINT uiThreadGroupCntZIn
)
	: ShaderResource(
		 uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS, 
		uiCPUAccessIn, uiMiscFlagIn, eUsageIn, eFormatIn
	), 
	uiThreadGroupCntX(uiThreadGroupCntXIn),
	uiThreadGroupCntY(uiThreadGroupCntYIn),
	uiThreadGroupCntZ(uiThreadGroupCntZIn)
{
	ID3D11Helper::CreateUnorderedAccessView(DirectXDevice::pDevice, cpTexture2D.Get(), cpUAV.GetAddressOf());
}

AFilter::~AFilter()
{
}

void AFilter::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpUAV.Reset();
	ShaderResource::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateUnorderedAccessView(
		DirectXDevice::pDevice, cpTexture2D.Get(), cpUAV.GetAddressOf()
	);
}
