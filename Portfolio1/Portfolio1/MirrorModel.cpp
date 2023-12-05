#include "MirrorModel.h"
#include "TransformProperties.h"
#include "CameraInterface.h"
#include "CameraInfo.h"
#include "ModelID.h"

#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace DirectX;

MirrorModel::MirrorModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fMirrorWidthIn,
	const float& fMirrorHeightIn,
	const float& fCenterXIn,
	const float& fCenterYIn,
	const float& fCenterZIn,
	const std::shared_ptr<CameraInterface>& spCameraIn
)
	: PickableModel(cpDeviceIn, cpDeviceContextIn), fMirrorWidth(fMirrorWidthIn), fMirrorHeight(fMirrorHeightIn),
	xmvDefaultDirection(XMVECTOR()), xmvDefaultUp(XMVECTOR()),
	spCamera(spCameraIn)
{
	AutoZeroMemory(sMirrorInfo);
	sMirrorInfo.fCenterPos.m128_f32[0] = fCenterXIn;
	sMirrorInfo.fCenterPos.m128_f32[1] = fCenterYIn;
	sMirrorInfo.fCenterPos.m128_f32[2] = fCenterZIn;

	PickableModel::MakePlaneVertexIndexSet(
		this, 
		xmvDefaultDirection, 
		xmvDefaultUp, 
		fCenterXIn, fCenterYIn, fCenterZIn, fMirrorWidth, fMirrorHeight);

	ID3D11Helper::CreateBuffer(cpDevice.Get(), sMirrorInfo, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL, cpMirrorInfoBuffer.GetAddressOf());
	
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), spCamera->uiWidth, spCamera->uiHeight, 1, 1, D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, spCamera->GetRenderedTextureFormat(), cpMirrorTexture.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpMirrorTexture.Get(), cpMirrorRTV.GetAddressOf());

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), spCamera->uiWidth, spCamera->uiHeight, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, spCamera->GetRenderedTextureFormat(), cpMirrorResolvedTexture.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpMirrorResolvedTexture.Get(), cpMirrorResolvedSRV.GetAddressOf());

	ID3D11Helper::CreateDepthStencilView(cpDevice.Get(), spCamera->uiWidth, spCamera->uiHeight, 1, cpMirrorDepthStencilTexture2D.GetAddressOf(), cpMirrorepthStencilView.GetAddressOf());
}

MirrorModel::~MirrorModel() {}

void MirrorModel::WipeOut(const DirectX::XMVECTOR& xmvClearColor)
{
	cpDeviceContext->ClearRenderTargetView(cpMirrorRTV.Get(), xmvClearColor.m128_f32);
	cpDeviceContext->ClearDepthStencilView(cpMirrorepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void MirrorModel::Update(const float& fDelta) 
{
	PickableModel::Update(fDelta);

	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(
		upTransformationProperties->sPositionAngle.fPitch,
		upTransformationProperties->sPositionAngle.fYaw,
		upTransformationProperties->sPositionAngle.fRoll
	);

	sMirrorInfo.fCenterPos = upTransformationProperties->xmvTranslation;

	XMVECTOR xmvModelDirect = XMVector4Transform(xmvDefaultDirection, xmRotationMat);
	XMVECTOR xmvModelUp = XMVector4Transform(xmvDefaultUp, xmRotationMat);

	XMVECTOR toMirror = XMVector4Normalize(sMirrorInfo.fCenterPos - spCamera->sCameraInfo.sCameraViewPosData.xmvCameraPosition);
	XMVECTOR reflectVec = MathematicalHelper::MakeReflect(xmvModelDirect, toMirror);
	float cosTheta = XMVector3Dot(xmvModelDirect, reflectVec).m128_f32[0];

	if (cosTheta >= 0)
	{
		sMirrorInfo.xmmMirrorViewProj = XMMatrixTranspose(XMMatrixLookToLH(sMirrorInfo.fCenterPos, reflectVec, xmvModelUp) *
			XMMatrixPerspectiveFovLH(70.f, fMirrorWidth / fMirrorHeight, 0.01f, 1000.f));
	}
	else
	{
		sMirrorInfo.xmmMirrorViewProj = XMMATRIX();
		sMirrorInfo.xmmMirrorViewProjInv = XMMATRIX();
	}

	ID3D11Helper::UpdateBuffer(cpDeviceContext.Get(), sMirrorInfo, D3D11_MAP_WRITE_DISCARD, cpMirrorInfoBuffer.Get());

	cpDeviceContext->ResolveSubresource(cpMirrorResolvedTexture.Get(), NULL, cpMirrorTexture.Get(), NULL, spCamera->GetRenderedTextureFormat());
}


void MirrorModel::SetIAProperties()
{
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void MirrorModel::SetVSConstantBuffers()
{
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, cpTransformationDataBuffer.GetAddressOf());
}

void MirrorModel::SetHSConstantBuffers()
{
}

void MirrorModel::SetDSConstantBuffers()
{
}

void MirrorModel::SetGSConstantBuffers()
{
}

void MirrorModel::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, upModelID->GetAddressOfTextureIDBuffer());
}

void MirrorModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, &pResetBuffer);
}

void MirrorModel::SetVSShaderResources()
{
}

void MirrorModel::SetHSShaderResources()
{
}

void MirrorModel::SetDSShaderResources()
{

}

void MirrorModel::SetGSShaderResources()
{

}


void MirrorModel::SetPSShaderResources()
{
	cpDeviceContext->PSSetShaderResources(PS_SRV_MIRROR_SELF, 1, cpMirrorResolvedSRV.GetAddressOf());
}

void MirrorModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->PSGetShaderResources(PS_SRV_MIRROR_SELF, 1, &pResetSRV);
}

void MirrorModel::SetMirrorConstantBuffer()
{
	// 거울의 위치와 뷰/프로젝트 행렬을 Camera를 대신하여 넣어준다.
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, cpMirrorInfoBuffer.GetAddressOf());
	cpDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, cpMirrorInfoBuffer.GetAddressOf());
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, cpMirrorInfoBuffer.GetAddressOf());
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, cpMirrorInfoBuffer.GetAddressOf());
}

void MirrorModel::ResetMirrorConstantBuffer()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
}


void MirrorModel::SetOMMirrorRenderTarget()
{
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpMirrorRTV.Get(), spCamera->cpModelIDRTV.Get() };
	cpDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpMirrorepthStencilView.Get());
}

void MirrorModel::ResetOMMirrorRenderTarget()
{
	ID3D11RenderTargetView* resetRTV = nullptr;
	cpDeviceContext->OMSetRenderTargets(1, &resetRTV, nullptr);
}
