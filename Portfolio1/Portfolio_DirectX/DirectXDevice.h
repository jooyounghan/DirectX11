#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <set>

class DirectXDevice
{
public:
	static ID3D11Device*		pDevice;
	static ID3D11DeviceContext* pDeviceContext;
	static IDXGISwapChain*		pSwapChain;

private:
	static Microsoft::WRL::ComPtr<ID3D11Device> cpDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> cpDeviceContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain> cpSwapChain;

public:
	static ID3D11Texture2D*			pBackBuffer;
	static ID3D11RenderTargetView*	pRenderTargetView;

private:
	static Microsoft::WRL::ComPtr<ID3D11Texture2D> cpBackBuffer;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cpRenderTargetView;

private:
	static ID3D11Debug* pDebug;
	static ID3D11InfoQueue* pDebugInfoQueue;
	static std::vector<D3D11_MESSAGE_ID>	vDebugMessages;

public:
	static ID3D11SamplerState** ppWrapSampler;
	static ID3D11SamplerState** ppClampSampler;
	static ID3D11SamplerState** ppBorderSampler;

private:
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> cpWrapSampler;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> cpClampSampler;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> cpBorderSampler;

public:
	static ID3D11BlendState* pAddingBlendState;

private:
	static Microsoft::WRL::ComPtr<ID3D11BlendState> cpAddingBlendState;

public:
	static ID3D11DepthStencilState* pDrawLessEqualDSS;

private:
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDrawLessEqualDSS;




public:
	static void InitDevice(
		IN const UINT& uiWidthIn,
		IN const UINT& uiHeightIn,
		IN const DXGI_FORMAT& eFormatIn,
		IN const bool& bWindowed,
		IN HWND hOutputWindow
	);

public:
	static void ResetBackBuffer();
	static void SetBackBuffer();

public:
	static void AddIgnoringMessageFilter(D3D11_MESSAGE_ID eMessage);
	static void RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID eMessage);

public:
	static void ApplyDebugMessageFilter();
};

