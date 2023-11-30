#include "MirrorModel.h"
#include "TransformProperties.h"
#include "CameraInterface.h"
#include "CameraInfo.h"

#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DefineVar.h"

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
}

MirrorModel::~MirrorModel() {}

void MirrorModel::Render()
{
}

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