#include "CameraHDR.h"

CameraHDR::CameraHDR(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn, const UINT& uiNumLevelQuality)
	: CameraBase(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, uiWidthIn, uiHeightIn, uiNumLevelQuality)
{
}

void CameraHDR::Update()
{
	sCameraInfo.Update();
}

void CameraHDR::Resize(const float& fAspectRatioIn)
{
}

void CameraHDR::WipeOut(const DirectX::XMVECTOR& xmvClearColor)
{
}

void CameraHDR::SetRSState()
{
}

void CameraHDR::SetVSConstantBuffers()
{
}

void CameraHDR::SetHSConstantBuffers()
{
}

void CameraHDR::SetDSConstantBuffers()
{
}

void CameraHDR::SetGSConstantBuffers()
{
}

void CameraHDR::SetPSConstantBuffers()
{
}

void CameraHDR::OMSetRenderTargets()
{
}
