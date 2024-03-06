#include "CubeRenderTargets.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

CubeDepthStencilView::CubeDepthStencilView(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiNumQualityLevelsIn
)
	: ShaderResource(
		uiWidthIn,
		uiHeightIn,
		6, uiNumQualityLevelsIn,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
		NULL,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS,
		DXGI_FORMAT_R32_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURECUBE
	),
	IRectangle(uiWidthIn, uiHeightIn)
{

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	AutoZeroMemory(dsvDesc);
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.ArraySize = 1;

	for (size_t idx = 0; idx < 6; ++idx)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = (UINT)idx;
		ID3D11Helper::CreateDepthStencilView(
			DirectXDevice::pDevice,
			cpTexture2D.Get(),
			&dsvDesc,
			cpDSVs[idx].GetAddressOf()
		);
	}
}

CubeDepthStencilView::~CubeDepthStencilView()
{
}
