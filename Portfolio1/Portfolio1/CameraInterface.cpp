#include "CameraInterface.h"
#include "ModelID.h"
#include "PostProcess.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

#include <algorithm>
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

const XMVECTOR	CameraInterface::DefaultDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
const XMVECTOR	CameraInterface::DefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
const XMVECTOR	CameraInterface::DefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

CameraInterface::CameraInterface(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQualityIn, const DXGI_FORMAT& eCameraFormatIn,
	const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
	const float& fFovDegreeIn, const float& fNearZIn, const float& fFarZIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn), pSwapChain(nullptr),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn), uiNumLevelQuality(uiNumLevelQualityIn), 
	eCameraFormat(eCameraFormatIn), uiMouseLocation{0, 0}, fRoll(0.f), fPitch(0.f), fYaw(0.f),
	fFovRadian(XMConvertToRadians(fFovDegreeIn)), fAspectRatio(uiWidth / (float)uiHeight), 
	fNearZ(fNearZIn), fFarZ(fFarZIn), fMoveSpeed(10.f), fMouseMovablePitchAngleDegree(XMConvertToRadians(90.f)), 
	fMouseMovableYawAngleDegree(XMConvertToRadians(360.f)), bMoveDirection{false, false, false, false}, 
	bFirstView(false), eBackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	AutoZeroMemory(sCameraViewport);
	AutoZeroMemory(sCameraViewProjData);
	sCameraViewProjData.xmvCameraPosition = XMVectorSet(fPosXIn, fPosYIn, fPosZIn, 0.f);
	SetRenderResources();
}

CameraInterface::~CameraInterface()
{
}

void CameraInterface::SetAsMainCamera(IDXGISwapChain* pSwapChainIn)
{
	cpBackBufferRTV.Reset();

	if (pSwapChainIn != nullptr)
	{
		pSwapChain = pSwapChainIn;
		if (!ID3D11Helper::GetBackBuffer(pSwapChain, cpBackBuffer.GetAddressOf()))
		{
			Console("Back Buffer를 불러오는데 실패하였습니다.");
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			cpBackBuffer->GetDesc(&desc);
			eBackBufferFormat = desc.Format;
			cout << desc.Width << " / " << desc.Height << endl;
			ID3D11Helper::CreateRenderTargetView(pDevice, cpBackBuffer.Get(), cpBackBufferRTV.GetAddressOf());
		}
	}
	else
	{
		eBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}

void CameraInterface::ResetFromMainCamera()
{
	pSwapChain = nullptr;
	cpBackBuffer.Reset();
	cpBackBufferRTV.Reset();
}

void CameraInterface::SetRenderResources()
{
	cpDepthStencilTexture2D.Reset();
	cpDepthStencilView.Reset();

	cpCameraOutputTexture.Reset();
	cpCameraOutputRTV.Reset();
	cpCameraOutputSRV.Reset();

	cpModelIDTexture.Reset();
	cpModelIDRTV.Reset();
	cpModelIDMSToSS.Reset();
	cpModelIDStagedTexture.Reset();

	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, pDeviceContext, &sCameraViewport);
	ID3D11Helper::CreateDepthStencilView(pDevice, uiWidth, uiHeight, uiNumLevelQuality, cpDepthStencilTexture2D.GetAddressOf(), cpDepthStencilView.GetAddressOf());
	ID3D11Helper::CreateBuffer(
		pDevice,
		sCameraViewProjData,
		D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL, cpCameraViewProjConstantBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateTexture2D(pDevice, uiWidth, uiHeight, 1, uiNumLevelQuality, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eCameraFormat, cpCameraOutputTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(pDevice, cpCameraOutputTexture.Get(), cpCameraOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(pDevice, cpCameraOutputTexture.Get(), cpCameraOutputSRV.GetAddressOf());

	ID3D11Helper::CreateTexture2D(pDevice, uiWidth, uiHeight, 1, uiNumLevelQuality, D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, eBackBufferFormat, cpModelIDTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(pDevice, cpModelIDTexture.Get(), cpModelIDRTV.GetAddressOf());
	ID3D11Helper::CreateTexture2D(pDevice, uiWidth, uiHeight, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eBackBufferFormat, cpModelIDMSToSS.GetAddressOf());
	ID3D11Helper::CreateTexture2D(pDevice, 1, 1, 1, 0, NULL, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, eBackBufferFormat, cpModelIDStagedTexture.GetAddressOf());
}

unsigned int CameraInterface::GetPointedModelIDAsRGBA()
{
	unsigned int uiResult = 0;
	if (cpModelIDStagedTexture.Get() && cpModelIDTexture.Get())
	{
		pDeviceContext->ResolveSubresource(cpModelIDMSToSS.Get(), 0, cpModelIDTexture.Get(), 0, eBackBufferFormat);

		D3D11_BOX sBox;
		AutoZeroMemory(sBox);
		sBox.left = uiMouseLocation[0];
		sBox.right = sBox.left + 1;
		sBox.top = uiMouseLocation[1];
		sBox.bottom = sBox.top + 1;
		sBox.front = 0;
		sBox.back = 1;

		pDeviceContext->CopySubresourceRegion(cpModelIDStagedTexture.Get(), 0, 0, 0, NULL, cpModelIDMSToSS.Get(), 0, &sBox);

		D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
		AutoZeroMemory(sMappedSubResource);
		pDeviceContext->Map(cpModelIDStagedTexture.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
		
		memcpy(&uiResult, sMappedSubResource.pData, sizeof(unsigned int));
		pDeviceContext->Unmap(cpModelIDStagedTexture.Get(), 0);
	}
	return uiResult;
}

void CameraInterface::DoPostProcess()
{
	if (upPostProcess != nullptr)
	{
		upPostProcess->Process(cpCameraOutputTexture.Get(), cpBackBuffer.Get(), cpBackBufferRTV.GetAddressOf());
	}
}


void CameraInterface::SetFromMouseXY(const int& iMouseX, const int& iMouseY)
{
	uiMouseLocation[0] = clamp((unsigned int)iMouseX, 0x0000u, (UINT)sCameraViewport.Width - 1);
	uiMouseLocation[1] = clamp((unsigned int)iMouseY, 0x0000u, (UINT)sCameraViewport.Height - 1);

	float fNdcX = uiMouseLocation[0] * 2.f / uiWidth - 1.f;
	float fNdcY = uiMouseLocation[1] * 2.f / uiHeight - 1.f;

	if (bFirstView)
	{
		fPitch = fMouseMovablePitchAngleDegree * fNdcY;
		fYaw = fMouseMovableYawAngleDegree * fNdcX;
	}
}

void CameraInterface::WipeOut()
{
	const float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (cpBackBufferRTV != nullptr)
	{
		pDeviceContext->ClearRenderTargetView(cpBackBufferRTV.Get(), clearColor);
	}
	pDeviceContext->ClearRenderTargetView(cpCameraOutputRTV.Get(), clearColor);
	pDeviceContext->ClearRenderTargetView(cpModelIDRTV.Get(), clearColor);
	pDeviceContext->ClearDepthStencilView(cpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CameraInterface::Update(const float& fDelta)
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);

	XMVECTOR xmvCameraDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvCameraUp = XMVector4Transform(DefaultUp, xmRotationMat);
	XMVECTOR xmvCameraRight = XMVector4Transform(DefaultRight, xmRotationMat);

	// 카메라 위치에 대한 업데이트
	sCameraViewProjData.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Forward]	? sCameraViewProjData.xmvCameraPosition + (fMoveSpeed * xmvCameraDirection * fDelta)	: sCameraViewProjData.xmvCameraPosition;
	sCameraViewProjData.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Left]		? sCameraViewProjData.xmvCameraPosition - (fMoveSpeed * xmvCameraRight * fDelta)		: sCameraViewProjData.xmvCameraPosition;
	sCameraViewProjData.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Backward]	? sCameraViewProjData.xmvCameraPosition - (fMoveSpeed * xmvCameraDirection * fDelta)	: sCameraViewProjData.xmvCameraPosition;
	sCameraViewProjData.xmvCameraPosition = bFirstView && bMoveDirection[MoveDir::Right]	? sCameraViewProjData.xmvCameraPosition + (fMoveSpeed * xmvCameraRight * fDelta)		: sCameraViewProjData.xmvCameraPosition;

	// 카메라 View Projection Matrix 업데이트
	XMMATRIX viewProjMatrix = MathematicalHelper::MakeViewProjMatrix(
		sCameraViewProjData.xmvCameraPosition,
		xmvCameraDirection, 
		xmvCameraUp,
		fFovRadian,
		fAspectRatio,
		fNearZ,
		fFarZ
	);

	sCameraViewProjData.xmmViewProjMat = XMMatrixTranspose(viewProjMatrix);
	sCameraViewProjData.xmmInvViewProjMat = XMMatrixInverse(nullptr, sCameraViewProjData.xmmViewProjMat);

	ID3D11Helper::UpdateBuffer(
		pDeviceContext,
		sCameraViewProjData,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		cpCameraViewProjConstantBuffer.Get()
	);

	if (upPostProcess != nullptr) upPostProcess->Update();
}

void CameraInterface::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn, const float& fAspectRatioIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;
	fAspectRatio = fAspectRatioIn;
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, pDeviceContext, &sCameraViewport);
	SetRenderResources();
}

