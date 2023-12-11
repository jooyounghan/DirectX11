#include "NormalCamera.h"
#include "ModelID.h"
#include "PostProcess.h"

#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

#include <vector>

using namespace std;
using namespace DirectX;

NormalCamera::NormalCamera(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn,
	const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
	const float& fFovDegreeIn, const float& fNearZIn, const float& fFarZIn
)
	: CameraInterface(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn, uiNumLevelQuality, eCameraFormatIn,
		fPosXIn, fPosYIn, fPosZIn, fFovDegreeIn, fNearZIn, fFarZIn
	)
{
	upPostProcess = make_unique<PostProcess>(pDevice, pDeviceContext, sCameraViewport, eCameraFormat, eBackBufferFormat);
}

NormalCamera::~NormalCamera()
{
}

void NormalCamera::Update(const float& fDelta)
{
	CameraInterface::Update(fDelta);
}

void NormalCamera::Resize(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn, 
	const float& fAspectRatioIn
)
{
	CameraInterface::Resize(uiWidthIn, uiHeightIn, fAspectRatioIn);

	upPostProcess.reset();
	upPostProcess = make_unique<PostProcess>(pDevice, pDeviceContext, sCameraViewport, eCameraFormat, eBackBufferFormat);
}


void NormalCamera::SetConstantBuffers()
{
	pDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	pDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	pDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	pDeviceContext->HSSetConstantBuffers(HS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
}

void NormalCamera::OMSetRenderTargets()
{
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpCameraOutputRTV.Get(), cpModelIDRTV.Get() };
	pDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpDepthStencilView.Get());
	pDeviceContext->RSSetViewports(1, &sCameraViewport);
}

void NormalCamera::ResetCamera()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	pDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	pDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	pDeviceContext->HSSetConstantBuffers(HS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);

	vector<ID3D11RenderTargetView*> vResetRTV{ nullptr, nullptr };
	pDeviceContext->OMSetRenderTargets(UINT(vResetRTV.size()), vResetRTV.data(), nullptr);
}