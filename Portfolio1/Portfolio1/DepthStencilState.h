#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class DepthStencilState
{
protected:
	DepthStencilState();

public:
	enum DepthStencilOption
	{
		MaskOption,
		DrawEqualOption,
		DrawNotEqualOption,
		DefaultOption,
	};

public:
	static void Init(IN ID3D11Device* pDevice);
	static ID3D11DepthStencilState* pGetDSS(IN DepthStencilOption eDSType);

protected:
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDefaultDSS;
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpMaskDSS;
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDrawEqualDSS;
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDrawNotEqualDSS;
	static void CreateDepthStencilState(IN DepthStencilOption eDSType, IN ID3D11Device* pDevice, OUT ID3D11DepthStencilState** ppDepthStencilState);

};

