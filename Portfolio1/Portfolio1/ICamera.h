#pragma once
#include "ID3D11Helper.h"
#include <directxmath/DirectXMath.h>

struct CameraInfo
{
	DirectX::XMVECTOR xmvCameraPosition;
	DirectX::XMVECTOR xmvCameraDirection;
	DirectX::XMVECTOR xmvCameraUp;
	float fFovAngle;
	float fAspectRatio;
	float fNearZ;
	float fFarZ;
};

namespace std {
	template<typename T>
	class shared_ptr;
}

class ICamera
{
public:
	ICamera(ComPtr<ID3D11Device>& cpDeviceIn,
		ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		ComPtr<IDXGISwapChain>& cpSwapChainIn, 
		const float& fAspectRatioIn
	);
	~ICamera() {};

public:
	void Update();
	void WipeOut(const float fcolor[4] = ICamera::DefaultClearColor);

public:
	const float&					fAspectRatio;
	CameraInfo						sCameraInfo;

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

