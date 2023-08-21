#include "ICamera.h"
#include "MathematicalHelper.h"
#include "EnumBuffer.h"

std::shared_ptr<ICamera>	ICamera::DefaultCamera = nullptr;
const float					ICamera::DefaultClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

using namespace DirectX;

ICamera::ICamera(ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn)
	: cpDevice(cpDeviceIn),
	cpDeviceContext(cpDeviceContextIn),
	cpSwapChain(cpSwapChainIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn),
	bFirstView(false)
{
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());
	ID3D11Helper::CreateRasterizerState(cpDevice.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpRasterizerState.GetAddressOf());

	sCameraInfo = CameraInfo::CreateCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight);
	XMMATRIX xmmViewProjTransposed = GetViewProjTransposed();
	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		xmmViewProjTransposed,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, 0,
		cpCameraConstantBuffer.GetAddressOf()
	);

}

void ICamera::Update()
{
	XMMATRIX xmmViewProjTransposed = GetViewProjTransposed();
	ID3D11Helper::UpdateBuffer(cpDeviceContext.Get(), xmmViewProjTransposed, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, cpCameraConstantBuffer.Get());
	cpDeviceContext->RSSetState(cpRasterizerState.Get());
	cpDeviceContext->VSSetConstantBuffers(ViewProjMatrix, 1, cpCameraConstantBuffer.GetAddressOf());
}

void ICamera::Resize(const float& fAspectRatioIn)
{
	sCameraInfo.fAspectRatio = fAspectRatioIn;
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	cpRenderTargetView.Reset();
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());
}

void ICamera::WipeOut(const float fcolor[4])
{
	cpDeviceContext->ClearRenderTargetView(cpRenderTargetView.Get(), fcolor);
	cpDeviceContext->OMSetRenderTargets(1, cpRenderTargetView.GetAddressOf(), nullptr);

}

void ICamera::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	float fNdcX = iMouseX * 2.f / uiWidth - 1.f;
	float fNdcY = iMouseY * 2.f / uiHeight - 1.f;

	fNdcX = std::clamp(fNdcX, -1.f, 1.f);
	fNdcY = std::clamp(fNdcY, -1.f, 1.f);

	if (bFirstView)
	{
		sCameraInfo.sCameraPose.fPitch = sCameraInfo.fMouseMovablePitchAngleDegree * fNdcY;
		sCameraInfo.sCameraPose.fYaw = sCameraInfo.fMouseMovableYawAngleDegree * fNdcX;
	}
	else
	{
		sCameraInfo.sCameraPose.fPitch = sCameraInfo.sCameraPose.fPitch;
		sCameraInfo.sCameraPose.fYaw = sCameraInfo.sCameraPose.fYaw;
	}
}

XMMATRIX ICamera::GetViewProjTransposed()
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(sCameraInfo.sCameraPose.fPitch, sCameraInfo.sCameraPose.fYaw, sCameraInfo.sCameraPose.fRoll);
	
	XMVECTOR xmvCameraDirection = XMVector4Transform(XMVectorSet(0.f, 0.f, 1.f, 0.f), xmRotationMat);
	XMVECTOR xmvCameraUp		= XMVector4Transform(XMVectorSet(0.f, 1.f, 0.f, 0.f), xmRotationMat);
	
	// DirectX11������ Row Major Order�� ���Ǳ� ������,
	// ��ġ ��ǥ ���� * �� ��� * �� ��� * �������� ����� ���°� �Ǿ�� �Ѵ�.
	// L * M * V * P(Row Major)

	// DirectX11���� ����ϴ� Row Major Order ������ �����Ͱ� HLSL�� �Ѿ�鼭
	// Column Major Order�� �����ͷ� ����ǹǷ�,
	// DirectX11���� ���� ��Ŀ� ��ġ�� ���־�� HLSL�� �Ѿ���� �����ϰ� ������ �� �ִ�.

	// ������ L ����, M, V, P ����� ��ġ�� ����Ͽ� HLSL�� �����־��ٰ� �Ҷ�,
	// HLSL���� �������� ��� * �� ��� * �� ��� * ��ġ ��ǥ ������ ���°� �Ǿ�� �Ѵ�.
	// P_T * V_T * M_T * L_T (Column Major)

	// (V * P)_T = P_T * V_T �̹Ƿ�,
	// CPU(DirectX11)���� V * P�� ����ϰ� ��ġ�� ó������ ������ GPU(HLSL)�� �����ش�.

	return XMMatrixTranspose(XMMatrixLookToLH(sCameraInfo.xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(sCameraInfo.fFovAngle, sCameraInfo.fAspectRatio, sCameraInfo.fNearZ, sCameraInfo.fFarZ));
}

void ICamera::SwitchFirstView()
{
	bFirstView = !bFirstView;
}

CameraInfo CameraInfo::CreateCameraInfo(
	IN const float& fPosX, 
	IN const float& fPosY,
	IN const float& fPosZ, 
	IN const float& fFovAngleDegreeIn, 
	IN const float& fAspectRatio, 
	IN const float& fNearZIn,
	IN const float& fFarZ, 
	IN const float& fMouseMovablePitchAngleDegreeIn,
	IN const float& fMouseMovableYawAngleDegreeIn
)
{
	CameraInfo sCameraInfo;
	AutoZeroMemory(sCameraInfo);
	sCameraInfo.xmvCameraPosition = XMVectorSet(fPosX, fPosY, fPosZ, 0.f);
	sCameraInfo.sCameraPose.fPitch = 0.f;
	sCameraInfo.sCameraPose.fRoll = 0.f;
	sCameraInfo.sCameraPose.fYaw = 0.f;
	sCameraInfo.fFovAngle = XMConvertToRadians(fFovAngleDegreeIn);
	sCameraInfo.fAspectRatio = fAspectRatio;
	sCameraInfo.fNearZ = fNearZIn;
	sCameraInfo.fFarZ = fFarZ;
	sCameraInfo.fMouseMovablePitchAngleDegree = XMConvertToRadians(fMouseMovablePitchAngleDegreeIn);
	sCameraInfo.fMouseMovableYawAngleDegree = XMConvertToRadians(fMouseMovableYawAngleDegreeIn);
	return sCameraInfo;
}
