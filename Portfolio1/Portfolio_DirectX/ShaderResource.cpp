#include "ShaderResource.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ShaderResource::ShaderResource()
	: Texture2D()
{
}

ShaderResource::ShaderResource(
	const UINT& uiHeightIn,
	const UINT& uiWidthIn,
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
	)
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
	)
{
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpSRV.ReleaseAndGetAddressOf();
	cpSRV = nullptr;
	Texture2D::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}
