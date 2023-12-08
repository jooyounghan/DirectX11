#include "MirrorModel.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"
#include "ModelID.h"

#include <vector>

using namespace std;
using namespace DirectX;

MirrorModel::MirrorModel(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const float& fMirrorWidthIn, const float& fMirrorHeightIn,
	DXGI_FORMAT eMirrorFormat,
	const float& fCenterXIn,
	const float& fCenterYIn,
	const float& fCenterZIn,
	const shared_ptr<CameraInterface>& cpCameraIn
)
	: PickableModelInterface(pDeviceIn, pDeviceContextIn),
	CameraInterface(
		pDeviceIn, pDeviceContextIn, 
		1000 * (fMirrorWidthIn / fMirrorHeightIn), 1000,
		1, eMirrorFormat, 
		fCenterXIn, fCenterYIn, fCenterZIn, 
		90.f, 0.01f, 1000.f
	), cpCamera(cpCameraIn),
	fMirrorWidth(fMirrorWidthIn), fMirrorHeight(fMirrorHeightIn), fMirrorAspectRatio(fMirrorWidthIn / fMirrorHeightIn)
{
	uiMirrorTextureWidth = UINT(1000.f * fMirrorAspectRatio);
	uiMirrorTextureHeight = 1000;

	PickableModelInterface::MakePlaneVertexIndexSet(this, fCenterXIn, fCenterYIn, fCenterZIn, fMirrorWidth, fMirrorHeight);

	ID3D11Helper::CreateTexture2D(
		PickableModelInterface::pDevice, uiMirrorTextureWidth, uiMirrorTextureHeight, 
		1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, 
		GetRenderedTextureFormat(), cpMirrorResolvedTexture.GetAddressOf()
	);
	ID3D11Helper::CreateShaderResoureView(
		PickableModelInterface::pDevice, 
		cpMirrorResolvedTexture.Get(), cpMirrorResolvedSRV.GetAddressOf()
	);
}

MirrorModel::~MirrorModel() {}


void MirrorModel::Update(const float& fDelta)
{
	PickableModelInterface::Update(fDelta);

	fPitch = sTransformation.sAngles.fPitch;
	fYaw = sTransformation.sAngles.fYaw;
	fRoll = sTransformation.sAngles.fRoll;

	sCameraViewProjData.xmvCameraPosition.m128_f32[0] = sTransformation.sTranslations.fX;
	sCameraViewProjData.xmvCameraPosition.m128_f32[1] = sTransformation.sTranslations.fY;
	sCameraViewProjData.xmvCameraPosition.m128_f32[2] = sTransformation.sTranslations.fZ;

	//CameraInterface의 업데이트를 재정의
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);
	XMVECTOR xmvMirrorDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvMirrorUp = XMVector4Transform(DefaultUp, xmRotationMat);

	XMVECTOR toMirror = XMVector4Normalize(sCameraViewProjData.xmvCameraPosition - cpCamera->sCameraViewProjData.xmvCameraPosition);
	XMVECTOR reflectVec = MathematicalHelper::MakeReflect(xmvMirrorDirection, toMirror);

	float cosTheta = XMVector3Dot(xmvMirrorDirection, reflectVec).m128_f32[0];

	if (cosTheta >= 0.f)
	{
		XMMATRIX viewProjMatrix = MathematicalHelper::MakeViewProjMatrix(
			sCameraViewProjData.xmvCameraPosition,
			reflectVec,
			xmvMirrorUp,
			fFovRadian,
			fAspectRatio,
			fNearZ,
			fFarZ
		);

		sCameraViewProjData.xmmViewProjMat = XMMatrixTranspose(viewProjMatrix);
		sCameraViewProjData.xmmInvViewProjMat = XMMatrixInverse(nullptr, sCameraViewProjData.xmmViewProjMat);
	}

	ID3D11Helper::UpdateBuffer(
		PickableModelInterface::pDeviceContext,
		sCameraViewProjData,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		cpCameraViewProjConstantBuffer.Get()
	);

	PickableModelInterface::pDeviceContext->ResolveSubresource(
		cpMirrorResolvedTexture.Get(), NULL, 
		cpCameraOutputTexture.Get(), NULL, 
		GetRenderedTextureFormat()
	);
}

void MirrorModel::SetIAProperties()
{
	PickableModelInterface::pDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	PickableModelInterface::pDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void MirrorModel::SetConstantBuffersAsModel()
{
	PickableModelInterface::pDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, cpTransformationBuffer.GetAddressOf());
	PickableModelInterface::pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, upModelID->GetAddressOfTextureIDBuffer());
}

void MirrorModel::SetShaderResourcesAsModel()
{
	PickableModelInterface::pDeviceContext->PSSetShaderResources(PS_SRV_MIRROR_SELF, 1, cpMirrorResolvedSRV.GetAddressOf());
}

void MirrorModel::ResetConstantBuffersAsModel()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	PickableModelInterface::pDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, &pResetBuffer);
	PickableModelInterface::pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, &pResetBuffer);
}

void MirrorModel::ResetShaderResourcesAsModel()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	PickableModelInterface::pDeviceContext->PSGetShaderResources(PS_SRV_MIRROR_SELF, 1, &pResetSRV);
}

void MirrorModel::SetConstantBuffersAsCamera()
{
	PickableModelInterface::pDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	PickableModelInterface::pDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	PickableModelInterface::pDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
	PickableModelInterface::pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, cpCameraViewProjConstantBuffer.GetAddressOf());
}

void MirrorModel::ResetConstantBuffersAsCamera()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	PickableModelInterface::pDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	PickableModelInterface::pDeviceContext->DSSetConstantBuffers(DS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	PickableModelInterface::pDeviceContext->GSSetConstantBuffers(GS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	PickableModelInterface::pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
}


void MirrorModel::OMSetRenderTargets()
{
	vector<ID3D11RenderTargetView*> vRenderTargetViews{ cpCameraOutputRTV.Get(), cpModelIDRTV.Get() };
	PickableModelInterface::pDeviceContext->OMSetRenderTargets(UINT(vRenderTargetViews.size()), vRenderTargetViews.data(), cpDepthStencilView.Get());
	PickableModelInterface::pDeviceContext->RSSetViewports(1, &sCameraViewport);
}

void MirrorModel::ResetCamera()
{
	ID3D11RenderTargetView* resetRTV = nullptr;
	PickableModelInterface::pDeviceContext->OMSetRenderTargets(1, &resetRTV, nullptr);
}
