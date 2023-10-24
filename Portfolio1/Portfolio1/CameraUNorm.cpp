#include "CameraUNorm.h"
#include "EnumVar.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"
#include "ModelID.h"
#include "PostProcess.h"

#include <algorithm>
#include <string>

using namespace std;
using namespace DirectX;

CameraUNorm::CameraUNorm(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn, const UINT& uiNumLevelQuality)
	: CameraInterface(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, uiWidthIn, uiHeightIn, uiNumLevelQuality)
{
	CreateSRDResource();
	CreateModelIDResource();
	SetPostProcess();
}

CameraUNorm::~CameraUNorm()
{
	if (pPostProcess != nullptr)
	{
		delete pPostProcess;
		pPostProcess = nullptr;
	}
}

void CameraUNorm::Update()
{
	sCameraInfo.Update();
	pPostProcess->Update();
}

void CameraUNorm::Resize(const float& fAspectRatioIn)
{
	sCameraInfo.sInfoData.fAspectRatio = fAspectRatioIn;
	cpBackBuffer.Reset();
	cpSDRRTV.Reset();
	cpModelIDTexture.Reset();
	cpModelIDRTV.Reset();

	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());

	CreateSRDResource();
	CreateModelIDResource();
}

void CameraUNorm::WipeOut(const DirectX::XMVECTOR& xmvClearColor)
{
	cpDeviceContext->ClearRenderTargetView(cpSwapChainRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearRenderTargetView(cpSDRRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearRenderTargetView(cpModelIDRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearDepthStencilView(cpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CameraUNorm::SetCameraProperty()
{
	sCameraInfo.SetCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight);
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);
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
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpSDRRTV.Get(), cpModelIDRTV.Get() };
	cpDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpDepthStencilView.Get());
}

DXGI_FORMAT CameraUNorm::GetRenderedTextureFormat()
{
	D3D11_TEXTURE2D_DESC sTextureDesc;
	cpSDRTexture->GetDesc(&sTextureDesc);
	return sTextureDesc.Format;
}

ID3D11Texture2D* CameraUNorm::GetRenderedTexture()
{
	return cpSDRTexture.Get();
}

ModelIDData CameraUNorm::GetPointedModelID()
{
	ModelIDData result;
	if (cpModelIDStagingTexture.Get() && cpModelIDTexture.Get())
	{
		cpDeviceContext->ResolveSubresource(cpModelIDMSToSS.Get(), 0, cpModelIDTexture.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);

		D3D11_BOX sBox;
		AutoZeroMemory(sBox);
		sBox.left = sCameraInfo.sInfoData.uiMouseLocation[0];
		sBox.right = sBox.left + 1;
		sBox.top = sCameraInfo.sInfoData.uiMouseLocation[1];
		sBox.bottom = sBox.top + 1;
		sBox.front = 0;
		sBox.back = 1;

		cpDeviceContext->CopySubresourceRegion(cpModelIDStagingTexture.Get(), 0, 0, 0, NULL, cpModelIDMSToSS.Get(), 0, &sBox);

		D3D11_TEXTURE2D_DESC test;
		cpModelIDMSToSS->GetDesc(&test);

		D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
		AutoZeroMemory(sMappedSubResource);
		cpDeviceContext->Map(cpModelIDStagingTexture.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
		unsigned int uiResult = 0;
		memcpy(&uiResult, sMappedSubResource.pData, sizeof(unsigned int));
		cpDeviceContext->Unmap(cpModelIDStagingTexture.Get(), 0);
		result = ModelID::ConvertR8G8B8A8ToModelID(uiResult);
	}
	return result;
}

void CameraUNorm::SetPostProcess()
{
	pPostProcess = new PostProcess(cpDevice, cpDeviceContext, sScreenViewport, DXGI_FORMAT_R8G8B8A8_UNORM);
	pPostProcess->AddBloomFilter();
	pPostProcess->AddBloomFilter();
}

void CameraUNorm::DoPostProcess()
{
	pPostProcess->Process(cpSDRTexture.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());
}

void CameraUNorm::CreateSRDResource()
{
	SetCameraProperty();
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpSDRTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpSDRTexture.Get(), cpSDRRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpSDRTexture.Get(), cpSDRSRV.GetAddressOf());
}

void CameraUNorm::CreateModelIDResource()
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, 1, D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpModelIDTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpModelIDTexture.Get(), cpModelIDRTV.GetAddressOf());

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpModelIDMSToSS.GetAddressOf());
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), 1, 1, 1, 0, NULL, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R8G8B8A8_UNORM, cpModelIDStagingTexture.GetAddressOf());

	ID3D11Helper::CreateDepthStencilView(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, cpDepthStencilTexture2D.GetAddressOf(), cpDepthStencilView.GetAddressOf());
}
