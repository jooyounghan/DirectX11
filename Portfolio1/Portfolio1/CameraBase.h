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
		const UINT& uiNumLevelQualityIn
	);
	~CameraBase();

public:
	UINT			uiWidth;
	UINT			uiHeight;
	UINT			uiNumLevelQuality;
	CameraInfo		sCameraInfo;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpSwapChainRTV;

public:
	void StartMove(MoveDir moveDir);
	void StopMove(MoveDir moveDir);
	void SetFromMouseXY(const int& iMouseX, const int& iMouseY);
	void SwitchFirstView();

public:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpRasterizerState;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChain;
};

