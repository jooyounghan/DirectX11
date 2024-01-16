#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class DepthStencilState
{
private:
	DepthStencilState(ID3D11Device* pDevice);
	DepthStencilState(const DepthStencilState& ref) {}
	DepthStencilState& operator=(const DepthStencilState& ref) {}
	~DepthStencilState() {}

public:
	enum DepthStencilOption
	{
		MaskOption,
		DrawEqualOption,
		DrawNotEqualOption,
		DefaultOption,
		DepthOnlyOption
	};

public:
	inline static DepthStencilState& GetInstance(ID3D11Device* pDevice)
	{
		static DepthStencilState s = DepthStencilState(pDevice);
		return s;
	}

public:
	ID3D11DepthStencilState* pGetDSS(IN DepthStencilOption eDSType);

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDefaultDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpMaskDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDrawEqualDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDrawNotEqualDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cpDepthOnlyDSS;

private:
	static void CreateDepthStencilState(IN DepthStencilOption eDSType, IN ID3D11Device* pDevice, OUT ID3D11DepthStencilState** ppDepthStencilState);
};

