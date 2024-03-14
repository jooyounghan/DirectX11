#include "CubeRenderTargets.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

CubeDepthStencilView::CubeDepthStencilView(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiNumQualityLevelsIn
)
	: ShaderResource(
		uiWidthIn,
		uiHeightIn,
		6, uiNumQualityLevelsIn,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		NULL,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS,
		DXGI_FORMAT_R32_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURECUBE
	),
	cubeViewParts {
		ViewableDepthOnly(fXPos, fYPos, fZPos, 0.f, 90.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn),
		ViewableDepthOnly(fXPos, fYPos, fZPos, 0.f, -90.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn),
		ViewableDepthOnly(fXPos, fYPos, fZPos, -90.f, 0.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn),
		ViewableDepthOnly(fXPos, fYPos, fZPos, 90.f, 0.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn),
		ViewableDepthOnly(fXPos, fYPos, fZPos, 0.f, 0.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn),
		ViewableDepthOnly(fXPos, fYPos, fZPos, 0.f, 180.f, 0.f, 90.f, fFallOffStartIn, fFallOffEndIn, uiWidthIn, uiHeightIn)
	},
	IRectangle(uiWidthIn, uiHeightIn)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	AutoZeroMemory(uavDesc);
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = 1;
	for (size_t idx = 0; idx < 6; ++idx)
	{
		uavDesc.Texture2DArray.FirstArraySlice = (UINT)idx;
		ID3D11Helper::CreateUnorderedAccessView(
			DirectXDevice::pDevice,
			cpTexture2D.Get(),
			&uavDesc,
			cpUAVs[idx].GetAddressOf()
		);
	}
}

CubeDepthStencilView::~CubeDepthStencilView()
{
}
