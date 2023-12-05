#include "CameraNormal.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"
#include "ModelID.h"
#include "PostProcess.h"
#include "ShaderTypeEnum.h"

#include <algorithm>
#include <string>

using namespace std;
using namespace DirectX;

CameraNormal::CameraNormal(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn
)
	: CameraInterface(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, uiWidthIn, uiHeightIn, uiNumLevelQuality, eCameraFormatIn)
{
	SetCameraProperty();
	CreateResource();
	CreateModelIDResource();
	SetPostProcess();
}

CameraNormal::~CameraNormal()
{
}

void CameraNormal::SetCameraProperty()
{
	sCameraInfo.SetCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight, 0.01f, 1000.f);
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);
}

void CameraNormal::Update(const float& fDelta)
{
	sCameraInfo.Update(fDelta);
	if (pPostProcess != nullptr) pPostProcess->Update();
}

void CameraNormal::Resize(const float& fAspectRatioIn)
{
	sCameraInfo.sCameraInteractionData.fAspectRatio = fAspectRatioIn;
	cpBackBuffer.Reset();
	cpCameraOutputRTV.Reset();
	cpModelIDTexture.Reset();
	cpModelIDRTV.Reset();

	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());

	CreateResource();
	CreateModelIDResource();
}


void CameraNormal::SetRSState()
{
	cpDeviceContext->RSSetState(cpRasterizerState.Get());
}

void CameraNormal::SetVSConstantBuffers()
{
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraNormal::SetHSConstantBuffers()
{
}

void CameraNormal::SetDSConstantBuffers()
{
	cpDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraNormal::SetGSConstantBuffers()
{
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, sCameraInfo.GetCameraInfoConstantBuffer().GetAddressOf());
}

void CameraNormal::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, sCameraInfo.cpCameraInfoConstantBuffer.GetAddressOf());
}

void CameraNormal::OMSetRenderTargets()
{
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpCameraOutputRTV.Get(), cpModelIDRTV.Get() };
	cpDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpDepthStencilView.Get());
}

void CameraNormal::ResetCamera()
{
	cpDeviceContext->RSSetState(nullptr);

	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);

	vector<ID3D11RenderTargetView*> vResetRTV{ nullptr, nullptr };
	cpDeviceContext->OMSetRenderTargets(UINT(vResetRTV.size()), vResetRTV.data(), nullptr);
}

ModelIDData CameraNormal::GetPointedModelID()
{
	ModelIDData result;
	if (cpModelIDStagingTexture.Get() && cpModelIDTexture.Get())
	{
		cpDeviceContext->ResolveSubresource(cpModelIDMSToSS.Get(), 0, cpModelIDTexture.Get(), 0, eBackBufferFormat);

		D3D11_BOX sBox;
		AutoZeroMemory(sBox);
		sBox.left = sCameraInfo.sCameraInteractionData.uiMouseLocation[0];
		sBox.right = sBox.left + 1;
		sBox.top = sCameraInfo.sCameraInteractionData.uiMouseLocation[1];
		sBox.bottom = sBox.top + 1;
		sBox.front = 0;
		sBox.back = 1;

		cpDeviceContext->CopySubresourceRegion(cpModelIDStagingTexture.Get(), 0, 0, 0, NULL, cpModelIDMSToSS.Get(), 0, &sBox);

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

void CameraNormal::CreateModelIDResource()
{

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, 1, uiNumLevelQuality, D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, eBackBufferFormat, cpModelIDTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpModelIDTexture.Get(), cpModelIDRTV.GetAddressOf());
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eBackBufferFormat, cpModelIDMSToSS.GetAddressOf());
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), 1, 1, 1, 0, NULL, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, eBackBufferFormat, cpModelIDStagingTexture.GetAddressOf());
}

void CameraNormal::SetPostProcess()
{
	pPostProcess = make_unique<PostProcess>(cpDevice, cpDeviceContext, sScreenViewport, eCameraFormat, eBackBufferFormat);
	pPostProcess->AddBloomFilter();
	pPostProcess->AddBloomFilter();
}

void CameraNormal::CreateResource()
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth, uiHeight, 1, uiNumLevelQuality, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eCameraFormat, cpCameraOutputTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpCameraOutputTexture.Get(), cpCameraOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpCameraOutputTexture.Get(), cpCameraOutputSRV.GetAddressOf());
}
