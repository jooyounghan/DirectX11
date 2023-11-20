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
	const UINT& uiNumLevelQualityIn, const DXGI_FORMAT& eCameraFormatIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), cpSwapChain(cpSwapChainIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn), uiNumLevelQuality(uiNumLevelQualityIn), 
	eCameraFormat(eCameraFormatIn), sCameraInfo(CameraInfo(cpDeviceIn, cpDeviceContextIn, uiWidthIn, uiHeightIn))
{
	AutoZeroMemory(sScreenViewport);
	ID3D11Texture2D* pBackBuffer;
	HRESULT hResult = cpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hResult))
	{
		Console("Back Buffer를 불러오는데 실패하였습니다.");
	}
	else
	{
		D3D11_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);
		eBackBufferFormat = desc.Format;
	}
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpSwapChainRTV.GetAddressOf());
	ID3D11Helper::CreateRasterizerState(cpDevice.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpRasterizerState.GetAddressOf());
	ID3D11Helper::CreateDepthStencilView(cpDevice.Get(), uiWidth, uiHeight, uiNumLevelQuality, cpDepthStencilTexture2D.GetAddressOf(), cpDepthStencilView.GetAddressOf());
}

CameraInterface::~CameraInterface()
{
}

void CameraInterface::StartMove(MoveDir moveDir)
{
	sCameraInfo.sCameraInteractionData.bMoveDirection[moveDir] = true;
}
void CameraInterface::StopMove(MoveDir moveDir)
{
	sCameraInfo.sCameraInteractionData.bMoveDirection[moveDir] = false;
}

void CameraInterface::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	sCameraInfo.sCameraInteractionData.uiMouseLocation[0] = clamp((unsigned int)iMouseX, 0x0000u, (UINT)sScreenViewport.Width - 1);
	sCameraInfo.sCameraInteractionData.uiMouseLocation[1] = clamp((unsigned int)iMouseY, 0x0000u, (UINT)sScreenViewport.Height - 1);

	float fNdcX = sCameraInfo.sCameraInteractionData.uiMouseLocation[0] * 2.f / uiWidth - 1.f;
	float fNdcY = sCameraInfo.sCameraInteractionData.uiMouseLocation[1] * 2.f / uiHeight - 1.f;

	if (sCameraInfo.sCameraInteractionData.bFirstView)
	{
		sCameraInfo.sCameraInteractionData.fPitch = sCameraInfo.sCameraInteractionData.fMouseMovablePitchAngleDegree * fNdcY;
		sCameraInfo.sCameraInteractionData.fYaw = sCameraInfo.sCameraInteractionData.fMouseMovableYawAngleDegree * fNdcX;
	}
	else
	{
		sCameraInfo.sCameraInteractionData.fPitch = sCameraInfo.sCameraInteractionData.fPitch;
		sCameraInfo.sCameraInteractionData.fYaw = sCameraInfo.sCameraInteractionData.fYaw;
	}
}

void CameraInterface::SwitchFirstView()
{
	sCameraInfo.sCameraInteractionData.bFirstView = !sCameraInfo.sCameraInteractionData.bFirstView;
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