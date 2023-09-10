#pragma once
#include <memory>
#include <directxmath/DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

#include "StructVar.h"
#include "EnumVar.h"

class CameraInfo
{
public:
	static CameraInfo CreateCameraInfo(
		IN const float& fPosX,
		IN const float& fPosY,
		IN const float& fPosZ,
		IN const float& fFovAngleDegreeIn,
		IN const float& fAspectRatioIn,
		IN const float& fNearZIn = 0.01f ,
		IN const float& fFarZIn = 100.f,
		IN const float& fMoveSpeedIn = 0.1f,
		IN const float& fMouseMovablePitchAngleDegreeIn = 90.f,
		IN const float& fMouseMovableYawAngleDegreeIn = 180.f
	);


public:
	DirectX::XMVECTOR	xmvCameraPosition;
	PositionAngle		sCameraPose;
	float				fFovAngle;
	float				fAspectRatio;
	float				fNearZ;
	float				fFarZ;
	float				fMoveSpeed;
	float				fMouseMovablePitchAngleDegree;
	float				fMouseMovableYawAngleDegree;
};

class ICamera
{
public:
	ICamera(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality
	);
	~ICamera() {};

public:
	void Update();
	void Resize(const float& fAspectRatioIn);
	void WipeOut(const float fcolor[4] = ICamera::DefaultClearColor);

public:
	void SetFromMouseXY(const int& iDeltaXIn, const int& iDeltaYIn);

public:
	UINT			uiWidth;
	UINT			uiHeight;
	CameraInfo		sCameraInfo;

public:
	DirectX::XMMATRIX GetViewProj(
		const DirectX::XMVECTOR& xmvCameraDirection,
		const DirectX::XMVECTOR& xmvCameraUp
		);

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cpCameraConstantBuffer;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpRasterizerState;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpModelIDRTV;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cpDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDepthStencilState;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChain;

protected:
	bool bFirstView;
	bool bMoveDirection[MoveDir::NUM_DIR];


public:
	void StartMove(MoveDir moveDir);
	void StopMove(MoveDir moveDir);
	void SwitchFirstView();

public:
	static std::shared_ptr<ICamera>			DefaultCamera;
	static const float						DefaultClearColor[4];

	static const DirectX::XMVECTOR			DefaultDirection;
	static const DirectX::XMVECTOR			DefaultUp;
	static const DirectX::XMVECTOR			DefaultRight;
};

