#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class RasterizerState
{
private:
	RasterizerState(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn);
	RasterizerState(const RasterizerState& ref) {}
	RasterizerState& operator=(const RasterizerState& ref) {}
	~RasterizerState() {}

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpSolidRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpWireframeState;

public:
	inline static RasterizerState& GetInstance(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	{
		static RasterizerState s = RasterizerState(pDeviceIn, pDeviceContextIn);
		return s;
	}

protected:
	ID3D11DeviceContext* pDeviceContext;

public:
	void SetWireFrameRS();
	void SetSolidRS();
};