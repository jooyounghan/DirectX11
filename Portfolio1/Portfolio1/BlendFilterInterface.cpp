#include "BlendFilterInterface.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"

BlendFilterInterface::BlendFilterInterface(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eFormat
)
	: FilterInterface(pDeviceIn, pDeviceContextIn, sScreenViewportIn)
{
	AutoZeroMemory(sBlendConstantData);
	ID3D11Helper::CreateTexture2D(
		pDevice,
		(UINT)sScreenViewport.Width, (UINT)sScreenViewport.Height, 
		1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 
		NULL, NULL, D3D11_USAGE_DEFAULT, eFormat, cpOutputTexture2D.GetAddressOf()
	);
	ID3D11Helper::CreateRenderTargetView(pDevice, cpOutputTexture2D.Get(), cpOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(pDevice, cpOutputTexture2D.Get(), cpOutputSRV.GetAddressOf());
	ID3D11Helper::CreateBuffer(pDevice, sBlendConstantData, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL, cpBlendConstantBuffer.GetAddressOf());
}

BlendFilterInterface::~BlendFilterInterface()
{
}

void BlendFilterInterface::Update()
{
	ID3D11Helper::UpdateBuffer(pDeviceContext, sBlendConstantData, D3D11_MAP_WRITE_DISCARD, cpBlendConstantBuffer.Get());
}
