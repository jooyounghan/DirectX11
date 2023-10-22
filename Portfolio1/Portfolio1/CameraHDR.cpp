#include "CameraHDR.h"
#include "PostProcess.h"
#include "ID3D11Helper.h"

CameraHDR::CameraHDR(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn, const UINT& uiNumLevelQuality)
	: CameraInterface(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, uiWidthIn, uiHeightIn, uiNumLevelQuality)
{
	//CreateModelIDResource();
	//SetPostProcess();
	//SetViewPort();
}

CameraHDR::~CameraHDR() {}

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

void CameraHDR::SetCameraProperty()
{
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);
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

DXGI_FORMAT CameraHDR::GetRenderedTextureFormat()
{
	return DXGI_FORMAT();
}

ID3D11Texture2D* CameraHDR::GetRenderedTexture()
{
	return nullptr;
}

void CameraHDR::SetPostProcess()
{
}

void CameraHDR::DoPostProcess()
{
	
}