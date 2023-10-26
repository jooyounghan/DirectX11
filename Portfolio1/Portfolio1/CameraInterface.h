#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include <memory>
#include "CameraInfo.h"

enum MoveDir;

class CameraInterface
{
public:
	CameraInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQualityIn
	);
	virtual ~CameraInterface();
	
public:
	UINT			uiWidth;
	UINT			uiHeight;
	UINT			uiNumLevelQuality;
	CameraInfo		sCameraInfo;

protected:
	D3D11_VIEWPORT sScreenViewport;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cpSwapChainRTV;

public:
	 inline ID3D11Texture2D* GetBackBufferTexture2D() { return cpBackBuffer.Get(); }
	 inline ID3D11RenderTargetView** GetAddressOfSwapChainRTV() { return cpSwapChainRTV.GetAddressOf(); }

public:
	virtual void SetCameraProperty() = 0;

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

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpDepthStencilView;

public:
	virtual void Update() = 0;
	virtual void Resize(const float&) = 0;
	virtual void WipeOut(const DirectX::XMVECTOR&) = 0;
	virtual void SetRSState() = 0;
	virtual void SetVSConstantBuffers() = 0;
	virtual void SetHSConstantBuffers() = 0;
	virtual void SetDSConstantBuffers() = 0;
	virtual void SetGSConstantBuffers() = 0;
	virtual void SetPSConstantBuffers() = 0;
	virtual void OMSetRenderTargets() = 0;

public:
	class PostProcess* pPostProcess = nullptr;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpModelIDRTV;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDMSToSS;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDStagingTexture;

public:
	virtual void SetPostProcess() = 0;
	virtual void DoPostProcess() = 0;

public:
	virtual DXGI_FORMAT GetRenderedTextureFormat() = 0;
	virtual ID3D11Texture2D* GetRenderedTexture() = 0;

public:
	virtual struct ModelIDData GetPointedModelID() = 0;

protected:
	virtual void CreateModelIDResource() = 0;
};