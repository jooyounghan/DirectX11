#include "CameraInterface.h"
#include "ModelID.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
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

	sCameraInfo.SetCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight);
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

void CameraInterface::SwitchFirstView()
{
	sCameraInfo.sInfoData.bFirstView = !sCameraInfo.sInfoData.bFirstView;
}
