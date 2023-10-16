#pragma once
#include <wrl/client.h>
#include <memory>
#include "CameraInfo.h"

class CameraBase
{
public:
	CameraBase(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality
	);
	~CameraBase();

public:
	UINT			uiWidth;
	UINT			uiHeight;
	CameraInfo		sCameraInfo;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpRenderTargetView;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpModelIDRTV;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpModelIDMSToSS;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpModelIDStagingTexture;

public:
	struct ModelIDData GetPointedModelID();

public:
	void StartMove(MoveDir moveDir);
	void StopMove(MoveDir moveDir);
	void SetFromMouseXY(const int& iMouseX, const int& iMouseY);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cpDepthStencilView;

public:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpRasterizerState;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChain;
};

