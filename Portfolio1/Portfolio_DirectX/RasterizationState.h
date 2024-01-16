#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class RasterizationState
{
private:
	RasterizationState(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn);
	RasterizationState(const RasterizationState& ref) {}
	RasterizationState& operator=(const RasterizationState& ref) {}
	~RasterizationState() {}

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpSolidRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	cpWireframeRS;
	ID3D11RasterizerState* pAppliedRS;
public:
	inline static RasterizationState& GetInstance(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	{
		static RasterizationState s = RasterizationState(pDeviceIn, pDeviceContextIn);
		return s;
	}

protected:
	ID3D11DeviceContext* pDeviceContext;

public:
	inline void SetSolidRS() { pAppliedRS = cpSolidRS.Get(); }
	inline void SetWireFrameRS() { pAppliedRS = cpWireframeRS.Get(); }

public:
	inline ID3D11RasterizerState* GetAppliedRS() { return pAppliedRS; }
	inline ID3D11RasterizerState* GetSolidRS() { return cpSolidRS.Get(); }
	inline ID3D11RasterizerState* GetWireFrameRS() { return cpWireframeRS.Get(); }
};