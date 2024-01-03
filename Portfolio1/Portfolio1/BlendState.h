#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

enum BlendStateOption
{
	DefaultBlendState,
	AddBlendState,
};

class BlendState
{
private:
	BlendState(ID3D11Device* pDevice);
	BlendState(const BlendState& ref) {}
	BlendState& operator=(const BlendState& ref) {}
	~BlendState() {}

public:
	inline static BlendState& GetInstance(ID3D11Device* pDevice)
	{
		static BlendState s = BlendState(pDevice);
		return s;
	}

public:
	ID3D11BlendState* pGetBS(IN BlendStateOption eBSType);

public:
	static float DefaultBlendFactor[4];

protected:
	Microsoft::WRL::ComPtr<ID3D11BlendState> cpDefaultBS;
	Microsoft::WRL::ComPtr<ID3D11BlendState> cpAdditionBS;

private:
	static void CreateBlendState(IN BlendStateOption eBSType, IN ID3D11Device* pDevice, OUT ID3D11BlendState** ppBlendState);
};

