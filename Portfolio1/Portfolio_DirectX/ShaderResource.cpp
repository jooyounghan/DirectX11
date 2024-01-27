#include "ShaderResource.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "ShaderResource.h"


ShaderResource::ShaderResource(
	const UINT& uiHeightIn,
	const UINT& uiWidthIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eTextureFormatIn,
	const DXGI_FORMAT& eSRVFormatIn,
	const D3D_SRV_DIMENSION& eDimensionIn
)
	: Texture2D(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE,
		uiCPUAccessIn,
		uiMiscFlagIn, 
		eUsageIn, 
		eTextureFormatIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
	AutoZeroMemory(sDesc);
	sDesc.Format = eSRVFormatIn;
	sDesc.ViewDimension = eDimensionIn;
	sDesc.Texture2D.MipLevels = 1;
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf(), &sDesc);
}

ShaderResource::ShaderResource(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn
)
	: Texture2D(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormatIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

ShaderResource::ShaderResource(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn,
	uint8_t* pImageSourceIn
)
	: Texture2D(
		uiWidthIn, uiHeightIn,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormatIn,
		pImageSourceIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpSRV.Reset();
	Texture2D::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateShaderResoureView(
		DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf()
	);
}

