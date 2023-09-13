#include "ICamera.h"
#include "MathematicalHelper.h"
#include "EnumVar.h"
#include "ID3D11Helper.h"

#include <string>

using namespace std;
using namespace DirectX;

shared_ptr<ICamera>	ICamera::DefaultCamera = nullptr;
const float					ICamera::DefaultClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
const XMVECTOR				ICamera::DefaultDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
const XMVECTOR				ICamera::DefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
const XMVECTOR				ICamera::DefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

ICamera::ICamera(ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	ComPtr<IDXGISwapChain>& cpSwapChainIn, const UINT& uiWidthIn, const UINT& uiHeightIn, const UINT& uiNumLevelQuality)
	: cpDevice(cpDeviceIn),
	cpDeviceContext(cpDeviceContextIn),
	cpSwapChain(cpSwapChainIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn),
	bFirstView(false), bMoveDirection{ false }
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

	//D3D11_DEPTH_STENCILOP_DESC sDepthOpDesc;
	//AutoZeroMemory(sDepthOpDesc);
	//D3D11_DEPTH_STENCILOP_DESC sStencilOpDesc;
	//AutoZeroMemory(sStencilOpDesc);
	//ID3D11Helper::CreateDepthStencilState(cpDevice.Get(), TRUE, D3D11_COMPARISON_LESS, TRUE, sDepthOpDesc, sStencilOpDesc, cpDepthStencilState.GetAddressOf());

	ID3D11Helper::CreateRasterizerState(cpDevice.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpRasterizerState.GetAddressOf());

	sCameraInfo = CameraInfo::CreateCameraInfo(0.f, 0.f, -10.f, 70.f, uiWidth / (float)uiHeight);


	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformedMatrix::CreateTransfomredMatrix(GetViewProj(DefaultDirection, DefaultUp)),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, 0,
		cpCameraConstantBuffer.GetAddressOf()
	);

}

void ICamera::Update()
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(sCameraInfo.sCameraPose.fPitch, sCameraInfo.sCameraPose.fYaw, sCameraInfo.sCameraPose.fRoll);

	XMVECTOR xmvCameraDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvCameraUp = XMVector4Transform(DefaultUp, xmRotationMat);
	XMVECTOR xmvCameraRight = XMVector4Transform(DefaultRight, xmRotationMat);

	// Key에 대한 업데이트
	sCameraInfo.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Forward] ? sCameraInfo.xmvCameraPosition + (sCameraInfo.fMoveSpeed * xmvCameraDirection) : sCameraInfo.xmvCameraPosition;
	sCameraInfo.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Left] ? sCameraInfo.xmvCameraPosition - (sCameraInfo.fMoveSpeed * xmvCameraRight) : sCameraInfo.xmvCameraPosition;
	sCameraInfo.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Backward] ? sCameraInfo.xmvCameraPosition - (sCameraInfo.fMoveSpeed * xmvCameraDirection) : sCameraInfo.xmvCameraPosition;
	sCameraInfo.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Right] ? sCameraInfo.xmvCameraPosition + (sCameraInfo.fMoveSpeed * xmvCameraRight) : sCameraInfo.xmvCameraPosition;

	// Mouse Angle에 대한 카메라 업데이트
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformedMatrix::CreateTransfomredMatrix(GetViewProj(xmvCameraDirection, xmvCameraUp)),
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		cpCameraConstantBuffer.Get()
	);

	// Camera에 대한 기본 데이터 업데이트
	cpDeviceContext->RSSetState(cpRasterizerState.Get());

	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ViewProjMatrix, 1, cpCameraConstantBuffer.GetAddressOf());
	cpDeviceContext->DSSetConstantBuffers(DSConstBufferType::DS_ViewProjMatrix, 1, cpCameraConstantBuffer.GetAddressOf());

	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpRenderTargetView.Get(), cpModelIDRTV.Get() };

	cpDeviceContext->OMSetRenderTargets(vRenderTargetViews.size(), vRenderTargetViews.data(), cpDepthStencilView.Get());
	//cpDeviceContext->OMSetDepthStencilState(cpDepthStencilState.Get(), 0);
}

void ICamera::Resize(const float& fAspectRatioIn)
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

void ICamera::WipeOut(const float fcolor[4])
{
	cpDeviceContext->ClearRenderTargetView(cpRenderTargetView.Get(), fcolor);
	cpDeviceContext->ClearRenderTargetView(cpModelIDRTV.Get(), fcolor);
	cpDeviceContext->ClearDepthStencilView(cpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void ICamera::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	sCameraInfo.uiMouseLocation[0] = iMouseX;
	sCameraInfo.uiMouseLocation[1] = iMouseY;

	sCameraInfo.uiMouseLocation[0] = clamp(sCameraInfo.uiMouseLocation[0], 0x0000u, 0xFFFFu);
	sCameraInfo.uiMouseLocation[1] = clamp(sCameraInfo.uiMouseLocation[1], 0x0000u, 0xFFFFu);

	float fNdcX = sCameraInfo.uiMouseLocation[0] * 2.f / uiWidth - 1.f;
	float fNdcY = sCameraInfo.uiMouseLocation[1] * 2.f / uiHeight - 1.f;

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

XMMATRIX ICamera::GetViewProj(
	const DirectX::XMVECTOR& xmvCameraDirection,
	const DirectX::XMVECTOR& xmvCameraUp
)
{
	// DirectX11에서는 Row Major Order가 사용되기 때문에,
	// 위치 좌표 벡터 * 모델 행렬 * 뷰 행렬 * 프로젝션 행렬의 형태가 되어야 한다.
	// L * M * V * P(Row Major)

	// DirectX11에서 사용하던 Row Major Order 형태의 데이터가 HLSL로 넘어가면서
	// Column Major Order의 데이터로 저장되므로,
	// DirectX11에서 기존 행렬에 전치를 해주어야 HLSL로 넘어갔을때 동일하게 연산할 수 있다.

	// 기존의 L 벡터, M, V, P 행렬을 전치할 경우하여 HLSL에 보내주었다고 할때,
	// HLSL에서 프로젝션 행렬 * 뷰 행렬 * 모델 행렬 * 위치 좌표 벡터의 형태가 되어야 한다.
	// P_T * V_T * M_T * L_T (Column Major)

	// (V * P)_T = P_T * V_T 이므로,
	// CPU(DirectX11)에서 V * P를 계산하고 전치를 처리해준 다음에 GPU(HLSL)로 보내준다.

	return XMMatrixLookToLH(sCameraInfo.xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(sCameraInfo.fFovAngle, sCameraInfo.fAspectRatio, sCameraInfo.fNearZ, sCameraInfo.fFarZ);
}

void ICamera::StartMove(MoveDir moveDir)
{
	bMoveDirection[moveDir] = true;
}
void ICamera::StopMove(MoveDir moveDir)
{
	bMoveDirection[moveDir] = false;
}

void ICamera::SwitchFirstView()
{
	bFirstView = !bFirstView;
}

unsigned int ICamera::GetPointedModelID()
{
	unsigned int uiResult = 0;
	if (cpModelIDStagingTexture.Get() && cpModelIDTexture.Get())
	{
		cpDeviceContext->ResolveSubresource(cpModelIDMSToSS.Get(), 0, cpModelIDTexture.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);

		D3D11_BOX sBox;
		AutoZeroMemory(sBox);
		sBox.left = sCameraInfo.uiMouseLocation[0];
		sBox.right = sBox.left + 1;
		sBox.top = sCameraInfo.uiMouseLocation[1];
		sBox.bottom = sBox.top + 1;
		sBox.front = 0;
		sBox.back = 1;

		cpDeviceContext->CopySubresourceRegion(cpModelIDStagingTexture.Get(), 0, 0, 0, NULL, cpModelIDMSToSS.Get(), 0, &sBox);

		D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
		AutoZeroMemory(sMappedSubResource);
		cpDeviceContext->Map(cpModelIDStagingTexture.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
		unsigned int uiIdArray[4];
		memcpy(&uiIdArray, sMappedSubResource.pData, sizeof(unsigned int) * 4);
		uiResult = uiIdArray[3];
		cpDeviceContext->Unmap(cpModelIDStagingTexture.Get(), 0);
	}
	return uiResult;
}

CameraInfo CameraInfo::CreateCameraInfo(
	IN const float& fPosX,
	IN const float& fPosY,
	IN const float& fPosZ,
	IN const float& fFovAngleDegreeIn,
	IN const float& fAspectRatioIn,
	IN const float& fNearZIn,
	IN const float& fFarZIn,
	IN const float& fMoveSpeedIn,
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
	sCameraInfo.fAspectRatio = fAspectRatioIn;
	sCameraInfo.fNearZ = fNearZIn;
	sCameraInfo.fFarZ = fFarZIn;
	sCameraInfo.fMoveSpeed = fMoveSpeedIn;
	sCameraInfo.fMouseMovablePitchAngleDegree = XMConvertToRadians(fMouseMovablePitchAngleDegreeIn);
	sCameraInfo.fMouseMovableYawAngleDegree = XMConvertToRadians(fMouseMovableYawAngleDegreeIn);
	return sCameraInfo;
}
