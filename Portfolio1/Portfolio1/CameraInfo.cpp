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
	AutoZeroMemory(sInfoData);
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
	sInfoData.xmvCameraPosition = XMVectorSet(fPosX, fPosY, fPosZ, 0.f);
	sInfoData.fRoll = 0.f;
	sInfoData.fPitch = 0.f;
	sInfoData.fYaw = 0.f;
	sInfoData.fFovAngle = XMConvertToRadians(fFovAngleDegreeIn);
	sInfoData.fAspectRatio = fAspectRatioIn;
	sInfoData.fNearZ = fNearZIn;
	sInfoData.fFarZ = fFarZIn;
	sInfoData.fMoveSpeed = fMoveSpeedIn;
	sInfoData.fMouseMovablePitchAngleDegree = XMConvertToRadians(fMouseMovablePitchAngleDegreeIn);
	sInfoData.fMouseMovableYawAngleDegree = XMConvertToRadians(fMouseMovableYawAngleDegreeIn);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformationBufferData(),
		D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL, cpCameraInfoConstantBuffer.GetAddressOf()
	);
}



void CameraInfo::Update()
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(sInfoData.fPitch, sInfoData.fYaw, sInfoData.fRoll);

	XMVECTOR xmvCameraDirection = XMVector4Transform(DefaultDirection, xmRotationMat);
	XMVECTOR xmvCameraUp = XMVector4Transform(DefaultUp, xmRotationMat);
	XMVECTOR xmvCameraRight = XMVector4Transform(DefaultRight, xmRotationMat);

	// Key�� ���� ������Ʈ
	sInfoData.xmvCameraPosition = sInfoData.bFirstView && sInfoData.bMoveDirection[MoveDir::Forward] ? sInfoData.xmvCameraPosition + (sInfoData.fMoveSpeed * xmvCameraDirection) : sInfoData.xmvCameraPosition;
	sInfoData.xmvCameraPosition = sInfoData.bFirstView && sInfoData.bMoveDirection[MoveDir::Left] ? sInfoData.xmvCameraPosition - (sInfoData.fMoveSpeed * xmvCameraRight) : sInfoData.xmvCameraPosition;
	sInfoData.xmvCameraPosition = sInfoData.bFirstView && sInfoData.bMoveDirection[MoveDir::Backward] ? sInfoData.xmvCameraPosition - (sInfoData.fMoveSpeed * xmvCameraDirection) : sInfoData.xmvCameraPosition;
	sInfoData.xmvCameraPosition = sInfoData.bFirstView && sInfoData.bMoveDirection[MoveDir::Right] ? sInfoData.xmvCameraPosition + (sInfoData.fMoveSpeed * xmvCameraRight) : sInfoData.xmvCameraPosition;

	// Mouse Angle�� ���� ī�޶� ������Ʈ
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

	return XMMatrixLookToLH(sInfoData.xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(sInfoData.fFovAngle, sInfoData.fAspectRatio, sInfoData.fNearZ, sInfoData.fFarZ);
}

