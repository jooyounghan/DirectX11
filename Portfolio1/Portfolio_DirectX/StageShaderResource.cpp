#include "StageShaderResource.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

StageShaderResource::StageShaderResource(
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eFormatIn
)
	: IShaderResource(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		NULL, D3D11_CPU_ACCESS_READ, NULL,
		D3D11_USAGE_STAGING, eFormatIn
	),
	IRectangle(
		uiWidthIn, uiHeightIn
	)
{
}

StageShaderResource::~StageShaderResource()
{
}

void StageShaderResource::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;

	cpSRV.Reset();
	cpTexture2D.Reset();

	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, uiWidth, uiHeight,
		uiArraySize, uiNumQualityLevels, 
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_CPU_ACCESS_READ, NULL,
		D3D11_USAGE_STAGING, 
		eFormat, cpTexture2D.GetAddressOf()
	);

	ID3D11Helper::CreateShaderResoureView(
		DirectXDevice::pDevice,
		cpTexture2D.Get(),
		cpSRV.GetAddressOf()
	);
}
