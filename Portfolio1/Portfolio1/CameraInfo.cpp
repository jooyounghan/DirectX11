#include "CameraInfo.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"

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
	AutoZeroMemory(sCameraInteractionData);
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
	sCameraViewPosData.xmvCameraPosition = XMVectorSet(fPosX, fPosY, fPosZ, 0.f);
	sCameraViewPosData.sTransformationBufferData = TransformationBufferData();

	sCameraInteractionData.fRoll = 0.f;
	sCameraInteractionData.fPitch = 0.f;
	sCameraInteractionData.fYaw = 0.f;
	sCameraInteractionData.fFovAngle = XMConvertToRadians(fFovAngleDegreeIn);
	sCameraInteractionData.fAspectRatio = fAspectRatioIn;
	sCameraInteractionData.fNearZ = fNearZIn;
	sCameraInteractionData.fFarZ = fFarZIn;
	sCameraInteractionData.fMoveSpeed = fMoveSpeedIn;
	sCameraInteractionData.fMouseMovablePitchAngleDegree = XMConvertToRadians(fMouseMovablePitchAngleDegreeIn);
	sCameraInteractionData.fMouseMovableYawAngleDegree = XMConvertToRadians(fMouseMovableYawAngleDegreeIn);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		sCameraViewPosData,
		D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL, cpCameraInfoConstantBuffer.GetAddressOf()
	);
}



void CameraInfo::Update(const float& fDelta)
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(sCameraInteractionData.fPitch, sCameraInteractionData.fYaw, sCameraInteractionData.fRoll);

	XMVECTOR xmvCameraDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvCameraUp = XMVector4Transform(DefaultUp, xmRotationMat);
	XMVECTOR xmvCameraRight = XMVector4Transform(DefaultRight, xmRotationMat);

	// Key�� ���� ������Ʈ
	sCameraViewPosData.xmvCameraPosition = sCameraInteractionData.bFirstView && sCameraInteractionData.bMoveDirection[MoveDir::Forward] ? sCameraViewPosData.xmvCameraPosition + (sCameraInteractionData.fMoveSpeed * xmvCameraDirection * fDelta) : sCameraViewPosData.xmvCameraPosition;
	sCameraViewPosData.xmvCameraPosition = sCameraInteractionData.bFirstView && sCameraInteractionData.bMoveDirection[MoveDir::Left] ? sCameraViewPosData.xmvCameraPosition - (sCameraInteractionData.fMoveSpeed * xmvCameraRight * fDelta) : sCameraViewPosData.xmvCameraPosition;
	sCameraViewPosData.xmvCameraPosition = sCameraInteractionData.bFirstView && sCameraInteractionData.bMoveDirection[MoveDir::Backward] ? sCameraViewPosData.xmvCameraPosition - (sCameraInteractionData.fMoveSpeed * xmvCameraDirection * fDelta) : sCameraViewPosData.xmvCameraPosition;
	sCameraViewPosData.xmvCameraPosition = sCameraInteractionData.bFirstView && sCameraInteractionData.bMoveDirection[MoveDir::Right] ? sCameraViewPosData.xmvCameraPosition + (sCameraInteractionData.fMoveSpeed * xmvCameraRight * fDelta) : sCameraViewPosData.xmvCameraPosition;

	// Mouse Angle�� ���� ī�޶� ������Ʈ
	XMMATRIX viewProjMatrix = GetViewProj(xmvCameraDirection, xmvCameraUp);
	TransformationBufferData::CreateTransfomredMatrix(viewProjMatrix, &sCameraViewPosData.sTransformationBufferData);

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sCameraViewPosData,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		cpCameraInfoConstantBuffer.Get()
	);
}

XMMATRIX CameraInfo::GetViewProj(
	const DirectX::XMVECTOR& xmvCameraDirection,
	const DirectX::XMVECTOR& xmvCameraUp
)
{
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

	return XMMatrixLookToLH(sCameraViewPosData.xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(sCameraInteractionData.fFovAngle, sCameraInteractionData.fAspectRatio, sCameraInteractionData.fNearZ, sCameraInteractionData.fFarZ);
}

