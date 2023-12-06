#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class SamplerState
{
private:
	SamplerState(ID3D11Device* pDevice);
	SamplerState(const SamplerState& ref) {}
	SamplerState& operator=(const SamplerState& ref) {}
	~SamplerState() {}

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpDrawerWrapSampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpDrawerClampSampler;

public:
	inline static SamplerState& GetInstance(ID3D11Device* pDevice)
	{
		static SamplerState s = SamplerState(pDevice);
		return s;
	}

public:
	inline ID3D11SamplerState** GetAddressOfWrapSampler() { return cpDrawerWrapSampler.GetAddressOf(); }
	inline ID3D11SamplerState** GetAddressOfClampSampler() { return cpDrawerClampSampler.GetAddressOf(); }
};