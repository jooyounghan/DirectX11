#include "MirrorModel.h"
#include "TransformProperties.h"
#include "CameraInterface.h"
#include "CameraInfo.h"
#include "ModelID.h"

#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"

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
	upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterXIn;
	upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterYIn;
	upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZIn;
	sMirrorInfo.fCenterPos.m128_f32[0] = fCenterXIn;
	sMirrorInfo.fCenterPos.m128_f32[1] = fCenterYIn;
	sMirrorInfo.fCenterPos.m128_f32[2] = fCenterZIn;

	PickableModel::MakePlaneVertexIndexSet(
		this, 
		xmvDefaultDirection, 
		xmvDefaultUp, 
		fCenterXIn, fCenterYIn, fCenterZIn, fMirrorWidth, fMirrorHeight);

	ID3D11Helper::CreateBuffer(cpDevice.Get(), sMirrorInfo, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL, cpMirrorInfoBuffer.GetAddressOf());
	
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), spCamera->uiWidth, spCamera->uiHeight, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, spCamera->GetRenderedTextureFormat(), cpMirrorTexture.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpMirrorTexture.Get(), cpMirrorSRV.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpMirrorTexture.Get(), cpMirrorRTV.GetAddressOf());
}

MirrorModel::~MirrorModel() {}

void MirrorModel::Update(const float& fDelta) 
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(
		upTransformationProperties->sPositionAngle.fPitch,
		upTransformationProperties->sPositionAngle.fYaw,
		upTransformationProperties->sPositionAngle.fRoll
	);

	xmvDefaultDirection = XMVector4Transform(xmvDefaultDirection, xmRotationMat);
	xmvDefaultUp = XMVector4Transform(xmvDefaultUp, xmRotationMat);


	XMVECTOR toMirror = sMirrorInfo.fCenterPos - spCamera->sCameraInfo.sCameraViewPosData.xmvCameraPosition;
	XMVECTOR reflectVec = MathematicalHelper::MakeReflect(xmvDefaultDirection, toMirror);
	float cosTheta = XMVector3Dot(xmvDefaultDirection, reflectVec).m128_f32[0];


	if (cosTheta >= 0)
	{
		sMirrorInfo.xmmMirrorViewProj = XMMatrixLookToLH(sMirrorInfo.fCenterPos, reflectVec, xmvDefaultUp) *
			XMMatrixOrthographicLH(fMirrorWidth, fMirrorHeight, 0.01f, 100.f);
		sMirrorInfo.xmmMirrorViewProjInv = XMMatrixInverse(nullptr, sMirrorInfo.xmmMirrorViewProj);
	}
	else
	{
		sMirrorInfo.xmmMirrorViewProj = XMMATRIX();
		sMirrorInfo.xmmMirrorViewProjInv = XMMATRIX();
	}

	ID3D11Helper::UpdateBuffer(cpDeviceContext.Get(), sMirrorInfo, D3D11_MAP_WRITE_DISCARD, cpMirrorInfoBuffer.Get());
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
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
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

}

void MirrorModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->PSGetShaderResources(PS_SRV_MIRROR_SELF, 1, &pResetSRV);
}

void MirrorModel::SetPSMirrorShaderResources()
{
	// 거울의 시선에서 그리는 Render Target View를 Shader Resource View로 이용한다.
	cpDeviceContext->PSGetShaderResources(PS_SRV_MIRROR_SELF, 1, cpMirrorSRV.GetAddressOf());
}

void MirrorModel::SetPSMirrorConstantBuffer()
{
	// 거울의 위치와 뷰/프로젝트 행렬을 Camera를 대신하여 넣어준다.
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, cpMirrorInfoBuffer.GetAddressOf());
}

void MirrorModel::ResetPSMirrorConstantBuffer()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(PS_SRV_MIRROR_SELF, 1, &pResetBuffer);
}

void MirrorModel::ResetPSMirrorShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->PSGetShaderResources(PS_CBUFF_CAMERA_INFO, 1, &pResetSRV);

}