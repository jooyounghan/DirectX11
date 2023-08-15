#include "ICamera.h"

#include <memory>

std::shared_ptr<ICamera>	ICamera::DefaultCamera = nullptr;
const float					ICamera::DefaultClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

ICamera::ICamera(ComPtr<ID3D11Device>& cpDeviceIn, 
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	ComPtr<IDXGISwapChain>& cpSwapChainIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), cpSwapChain(cpSwapChainIn)
{
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());
}

void ICamera::WipeOut(const float fcolor[4])
{
	cpDeviceContext->ClearRenderTargetView(cpRenderTargetView.Get(), fcolor);
	cpDeviceContext->OMSetRenderTargets(1, cpRenderTargetView.GetAddressOf(), nullptr);
}

