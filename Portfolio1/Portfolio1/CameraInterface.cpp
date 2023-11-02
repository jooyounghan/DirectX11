#include "CameraInterface.h"
#include "ModelID.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
#include "PostProcess.h"

#include <algorithm>

using namespace std;

CameraInterface::CameraInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQualityIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), cpSwapChain(cpSwapChainIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn), uiNumLevelQuality(uiNumLevelQualityIn), sCameraInfo(CameraInfo(cpDeviceIn, cpDeviceContextIn, uiWidthIn, uiHeightIn))
{
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());
	ID3D11Helper::CreateRasterizerState(cpDevice.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpRasterizerState.GetAddressOf());
	ID3D11Helper::CreateDepthStencilView(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, cpDepthStencilTexture2D.GetAddressOf(), cpDepthStencilView.GetAddressOf());
}

CameraInterface::~CameraInterface()
{
	if (pPostProcess != nullptr)
	{
		delete pPostProcess;
		pPostProcess = nullptr;
	}
}

void CameraInterface::StartMove(MoveDir moveDir)
{
	sCameraInfo.sInfoData.bMoveDirection[moveDir] = true;
}
void CameraInterface::StopMove(MoveDir moveDir)
{
	sCameraInfo.sInfoData.bMoveDirection[moveDir] = false;
}

void CameraInterface::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	sCameraInfo.sInfoData.uiMouseLocation[0] = clamp((unsigned int)iMouseX, 0x0000u, (UINT)sScreenViewport.Width - 1);
	sCameraInfo.sInfoData.uiMouseLocation[1] = clamp((unsigned int)iMouseY, 0x0000u, (UINT)sScreenViewport.Height - 1);

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

void CameraInterface::SwitchFirstView()
{
	sCameraInfo.sInfoData.bFirstView = !sCameraInfo.sInfoData.bFirstView;
}


void CameraInterface::WipeOut(const DirectX::XMVECTOR& xmvClearColor)
{
	cpDeviceContext->ClearRenderTargetView(cpSwapChainRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearRenderTargetView(cpCameraOutputRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearRenderTargetView(cpModelIDRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearDepthStencilView(cpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

DXGI_FORMAT CameraInterface::GetRenderedTextureFormat()
{
	D3D11_TEXTURE2D_DESC sTextureDesc;
	cpCameraOutputTexture->GetDesc(&sTextureDesc);
	return sTextureDesc.Format;
}

void CameraInterface::DoPostProcess()
{
	if (pPostProcess != nullptr)
	{
		pPostProcess->Process(cpCameraOutputTexture.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());
	}
}