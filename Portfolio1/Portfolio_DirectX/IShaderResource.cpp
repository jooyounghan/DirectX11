#include "IShaderResource.h"
#include "ID3D11Helper.h"

IShaderResource::IShaderResource(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiHeightIn,
	const UINT& uiWidthIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn
)
	: ITexture2D(
		pDeviceIn, 
		pDeviceContextIn, 
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE,
		uiCPUAccessIn,
		uiMiscFlagIn, 
		eUsageIn, 
		eFormatIn
	)
{
	ID3D11Helper::CreateShaderResoureView(pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

IShaderResource::IShaderResource(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn,
	const uint8_t* const pImageSourceIn
)
	: ITexture2D(
		pDeviceIn,
		pDeviceContextIn,
		uiWidthIn, uiHeightIn,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormatIn,
		pImageSourceIn
	)
{
	ID3D11Helper::CreateShaderResoureView(pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

IShaderResource::~IShaderResource()
{
}
