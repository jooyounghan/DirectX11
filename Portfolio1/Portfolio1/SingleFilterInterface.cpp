#include "SingleFilterInterface.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"

SingleFilterInterface::SingleFilterInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eFormat
)
	: FilterInterface(cpDeviceIn, cpDeviceContextIn, sScreenViewportIn)
{
	AutoZeroMemory(sViewportInfoData);
	sViewportInfoData.fDeltaWidth = 1 / sScreenViewport.Width;
	sViewportInfoData.fDeltaHeight = 1 / sScreenViewport.Height;
	ID3D11Helper::CreateTexture2D(
		cpDevice.Get(),
		(UINT)sScreenViewport.Width, (UINT)sScreenViewport.Height,
		1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		NULL, NULL, D3D11_USAGE_DEFAULT, eFormat, cpOutputTexture2D.GetAddressOf()
	);
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputSRV.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), sViewportInfoData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER, NULL, NULL, cpViewportConstantBuffer.GetAddressOf());
}

SingleFilterInterface::~SingleFilterInterface()
{
}