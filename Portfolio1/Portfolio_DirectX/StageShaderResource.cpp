#include "StageShaderResource.h"
#include "ID3D11Helper.h"

StageShaderResource::StageShaderResource(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eFormatIn
)
	: IShaderResource(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		NULL, D3D11_CPU_ACCESS_READ, NULL,
		D3D11_USAGE_STAGING, eFormatIn
	),
	IDirectXDevice(
		pDeviceIn,
		pDeviceContextIn
	),
	IRectangle(
		uiWidthIn, uiHeightIn, uiArraySizeIn, uiNumQualityLevelsIn
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
		pDevice, uiWidth, uiHeight, 
		uiArraySize, uiNumQualityLevels, 
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_CPU_ACCESS_READ, NULL,
		D3D11_USAGE_STAGING, 
		eFormat, cpTexture2D.GetAddressOf()
	);

	ID3D11Helper::CreateShaderResoureView(
		pDevice, 
		cpTexture2D.Get(),
		cpSRV.GetAddressOf()
	);
}
