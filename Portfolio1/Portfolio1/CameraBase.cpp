#include "CameraBase.h"
#include "ModelID.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
#include <algorithm>

using namespace std;

CameraBase::CameraBase(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQuality
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), cpSwapChain(cpSwapChainIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn), sCameraInfo(CameraInfo(cpDeviceIn, cpDeviceContextIn, uiWidthIn, uiHeightIn))
{
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());

	D3D11_TEXTURE2D_DESC desc;
	cpBackBuffer->GetDesc(&desc);

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), desc, cpModelIDTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpModelIDTexture.Get(), cpModelIDRTV.GetAddressOf());

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), desc.Width, desc.Height, 1, 1, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpModelIDMSToSS.GetAddressOf());
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), 1, 1, 1, 1, NULL, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R8G8B8A8_UNORM, cpModelIDStagingTexture.GetAddressOf());

	ID3D11Helper::CreateDepthStencilView(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, cpDepthStencilTexture2D.GetAddressOf(), cpDepthStencilView.GetAddressOf());

	ID3D11Helper::CreateRasterizerState(cpDevice.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpRasterizerState.GetAddressOf());

	sCameraInfo.SetCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight);
}

CameraBase::~CameraBase() {}

ModelIDData CameraBase::GetPointedModelID()
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

void CameraBase::StartMove(MoveDir moveDir)
{
	sCameraInfo.sInfoData.bMoveDirection[moveDir] = true;
}
void CameraBase::StopMove(MoveDir moveDir)
{
	sCameraInfo.sInfoData.bMoveDirection[moveDir] = false;
}

void CameraBase::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	sCameraInfo.sInfoData.uiMouseLocation[0] = clamp((unsigned int)iMouseX, 0x0000u, 0xFFFFu);
	sCameraInfo.sInfoData.uiMouseLocation[1] = clamp((unsigned int)iMouseY, 0x0000u, 0xFFFFu);

	float fNdcX = sCameraInfo.sInfoData.uiMouseLocation[0] * 2.f / uiWidth - 1.f;
	float fNdcY = sCameraInfo.sInfoData.uiMouseLocation[1] * 2.f / uiHeight - 1.f;

	if (sCameraInfo.sInfoData.bFirstView)
	{
		sCameraInfo.sInfoData.fPitch = sCameraInfo.sInfoData.fMouseMovablePitchAngleDegree * fNdcY;
		sCameraInfo.sInfoData.fYaw = sCameraInfo.sInfoData.fMouseMovableYawAngleDegree * fNdcX;
	}
	else
	{
		sCameraInfo.sInfoData.fPitch = sCameraInfo.sInfoData.fPitch;
		sCameraInfo.sInfoData.fYaw = sCameraInfo.sInfoData.fYaw;
	}
}

void CameraBase::SwitchFirstView()
{
	sCameraInfo.sInfoData.bFirstView = !sCameraInfo.sInfoData.bFirstView;
}
