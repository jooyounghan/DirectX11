#include "CameraInfo.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"

using namespace DirectX;
using namespace Microsoft::WRL;

const XMVECTOR	CameraInfo::DefaultDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
const XMVECTOR	CameraInfo::DefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
const XMVECTOR	CameraInfo::DefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);



CameraInfo::CameraInfo(
	IN Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	IN Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	IN const UINT& uiWidthIn,
	IN const UINT& uiHeightIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn),
	uiWidth(uiWidthIn), uiHeight(uiHeightIn)
{

}

void CameraInfo::SetCameraInfo(
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
	xmvCameraPosition = XMVectorSet(fPosX, fPosY, fPosZ, 0.f);
	fRoll = 0.f;
	fPitch = 0.f;
	fYaw = 0.f;
	fFovAngle = XMConvertToRadians(fFovAngleDegreeIn);
	fAspectRatio = fAspectRatioIn;
	fNearZ = fNearZIn;
	fFarZ = fFarZIn;
	fMoveSpeed = fMoveSpeedIn;
	fMouseMovablePitchAngleDegree = XMConvertToRadians(fMouseMovablePitchAngleDegreeIn);
	fMouseMovableYawAngleDegree = XMConvertToRadians(fMouseMovableYawAngleDegreeIn);
}



void CameraInfo::Update()
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);

	XMVECTOR xmvCameraDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvCameraUp = XMVector4Transform(DefaultUp, xmRotationMat);
	XMVECTOR xmvCameraRight = XMVector4Transform(DefaultRight, xmRotationMat);

	// Key에 대한 업데이트
	xmvCameraPosition = bMoveDirection[MoveDir::Forward] ? xmvCameraPosition + (fMoveSpeed * xmvCameraDirection) : xmvCameraPosition;
	xmvCameraPosition = bMoveDirection[MoveDir::Left] ? xmvCameraPosition - (fMoveSpeed * xmvCameraRight) : xmvCameraPosition;
	xmvCameraPosition = bMoveDirection[MoveDir::Backward] ? xmvCameraPosition - (fMoveSpeed * xmvCameraDirection) : xmvCameraPosition;
	xmvCameraPosition = bMoveDirection[MoveDir::Right] ? xmvCameraPosition + (fMoveSpeed * xmvCameraRight) : xmvCameraPosition;

	// Mouse Angle에 대한 카메라 업데이트
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(GetViewProj(xmvCameraDirection, xmvCameraUp)),
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		cpCameraInfoConstantBuffer.Get()
	);
}

XMMATRIX CameraInfo::GetViewProj(
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

	return XMMatrixLookToLH(xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(fFovAngle, fAspectRatio, fNearZ, fFarZ);
}

