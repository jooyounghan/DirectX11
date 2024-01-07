#include "IFilter.h"
#include "ID3D11Helper.h"

IFilter::IFilter(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
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
	: IShaderResource(
		pDeviceIn, uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	), pDeviceContext(pDeviceContextIn),
	uiThreadGroupCntX(uiThreadGroupCntXIn),
	uiThreadGroupCntY(uiThreadGroupCntYIn),
	uiThreadGroupCntZ(uiThreadGroupCntZIn)
{
	ID3D11Helper::CreateUnorderedAccessView(pDeviceIn, cpTexture2D.Get(), cpUAV.GetAddressOf());
}

IFilter::~IFilter()
{
}
