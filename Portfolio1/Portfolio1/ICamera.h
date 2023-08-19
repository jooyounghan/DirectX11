#pragma once

#include <memory>
#include "StructVar.h"
#include "ID3D11Helper.h"
#include <directxmath/DirectXMath.h>

class CameraInfo
{
public:
	static CameraInfo CreateCameraInfo(
		IN const float& fPosX,
		IN const float& fPosY,
		IN const float& fPosZ,
		IN const float& fFovAngleDegreeIn,
		IN const float& fAspectRatio,
		IN const float& fNearZIn = 0.01f ,
		IN const float& fFarZ = 100.f,
		IN const float& fMouseMovableAngleDegreeIn = 180.f
	);


public:
	DirectX::XMVECTOR	xmvCameraPosition;
	PositionAngle		sCameraPose;
	float				fFovAngle;
	float				fAspectRatio;
	float				fNearZ;
	float				fFarZ;
	float				fMouseMovableAngle;
};

class ICamera
{
public:
	ICamera(ComPtr<ID3D11Device>& cpDeviceIn,
		ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		ComPtr<IDXGISwapChain>& cpSwapChainIn, 
		const UINT& uiWidthIn, const UINT& uiHeightIn
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
	DirectX::XMMATRIX GetViewProjTransposed();

public:
	ComPtr<ID3D11Buffer>			cpCameraConstantBuffer;

private:
	ComPtr<ID3D11Texture2D>			cpBackBuffer;
	ComPtr<ID3D11RenderTargetView>	cpRenderTargetView;
	ComPtr<ID3D11RasterizerState>	cpRasterizerState;

private:
	ComPtr<ID3D11Device>& cpDevice;
	ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	ComPtr<IDXGISwapChain>& cpSwapChain;

public:
	static std::shared_ptr<ICamera> DefaultCamera;
	static const float				DefaultClearColor[4];
};

