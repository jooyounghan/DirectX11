#pragma once

#include <directxmath/DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include "EnumVar.h"

class CameraInfo
{
public:
	CameraInfo(
		IN Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		IN Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		IN const UINT& uiWidthIn,
		IN const UINT& uiHeightIn
	);

public:
	void SetCameraInfo(
		IN const float& fPosX,
		IN const float& fPosY,
		IN const float& fPosZ,
		IN const float& fFovAngleDegreeIn,
		IN const float& fAspectRatioIn,
		IN const float& fNearZIn = 0.01f,
		IN const float& fFarZIn = 100.f,
		IN const float& fMoveSpeedIn = 0.1f,
		IN const float& fMouseMovablePitchAngleDegreeIn = 90.f,
		IN const float& fMouseMovableYawAngleDegreeIn = 180.f
	);

public:
	struct {
		unsigned int			uiMouseLocation[2];
		DirectX::XMVECTOR		xmvCameraPosition;

		float	fRoll;
		float	fPitch;
		float	fYaw;
		float	fFovAngle;
		float	fAspectRatio;
		float	fNearZ;
		float	fFarZ;
		float	fMoveSpeed;
		float	fMouseMovablePitchAngleDegree;
		float	fMouseMovableYawAngleDegree;
		bool			bMoveDirection[MoveDir::NUM_DIR];
		bool			bFirstView;
	} sInfoData;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpCameraInfoConstantBuffer;

public:
	inline const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetCameraInfoConstantBuffer() { return cpCameraInfoConstantBuffer; }

public:
	void Update();

public:
	DirectX::XMMATRIX GetViewProj(
		const DirectX::XMVECTOR& xmvCameraDirection,
		const DirectX::XMVECTOR& xmvCameraUp
	);

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	UINT uiWidth;
	UINT uiHeight;

private:
	static const DirectX::XMVECTOR			DefaultDirection;
	static const DirectX::XMVECTOR			DefaultUp;
	static const DirectX::XMVECTOR			DefaultRight;
};
