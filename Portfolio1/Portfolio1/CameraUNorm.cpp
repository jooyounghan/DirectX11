#include "CameraUNorm.h"
#include "EnumVar.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"
#include "ModelID.h"

#include <algorithm>
#include <string>

using namespace std;
using namespace DirectX;

CameraUNorm::CameraUNorm(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn, const UINT& uiNumLevelQuality)
	: CameraBase(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, uiWidthIn, uiHeightIn, uiNumLevelQuality)
{

}

void CameraUNorm::Update()
{
	sCameraInfo.Update();
}

void CameraUNorm::Resize(const float& fAspectRatioIn)
{
	sCameraInfo.fAspectRatio = fAspectRatioIn;
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	cpRenderTargetView.Reset();
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());

	D3D11_TEXTURE2D_DESC desc;
	cpBackBuffer->GetDesc(&desc);
	cpModelIDTexture.Reset();
	cpModelIDRTV.Reset();
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), desc, cpModelIDTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpModelIDTexture.Get(), cpModelIDRTV.GetAddressOf());
}

void CameraUNorm::WipeOut(const DirectX::XMVECTOR& xmvClearColor)
{
	cpDeviceContext->ClearRenderTargetView(cpRenderTargetView.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearRenderTargetView(cpModelIDRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearDepthStencilView(cpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CameraUNorm::SetRSState()
{
	cpDeviceContext->RSSetState(cpRasterizerState.Get());
}

void CameraUNorm::SetVSConstantBuffers()
{
	
	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ViewProjMatrix, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraUNorm::SetHSConstantBuffers()
{
}

void CameraUNorm::SetDSConstantBuffers()
{
	cpDeviceContext->DSSetConstantBuffers(DSConstBufferType::DS_ViewProjMatrix, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraUNorm::SetGSConstantBuffers()
{
	cpDeviceContext->GSSetConstantBuffers(GSConstBufferType::GS_ViewProjMatrix, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraUNorm::SetPSConstantBuffers()
{
}

void CameraUNorm::OMSetRenderTargets()
{
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpRenderTargetView.Get(), cpModelIDRTV.Get() };
	cpDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpDepthStencilView.Get());
}
