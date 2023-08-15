#pragma once
#include "ID3D11Helper.h"

namespace std {
	template<typename T>
	class shared_ptr;
}

class ICamera
{
public:
	ICamera(ComPtr<ID3D11Device>& cpDeviceIn,
		ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		ComPtr<IDXGISwapChain>& cpSwapChainIn
	);
	~ICamera() {};

public:
	void SetMain();
	void WipeOut(const float fcolor[4] = ICamera::DefaultClearColor);

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

