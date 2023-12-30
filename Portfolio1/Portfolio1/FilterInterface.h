#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>

#include "ManualDataType.h"

struct FilterVertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
};

class FilterInfo
{
private:
	FilterInfo(ID3D11Device* pDeviceIn);
	FilterInfo(const FilterInfo& ref){}
	FilterInfo& operator=(const FilterInfo& ref) {}
	~FilterInfo() {}

public:
	static FilterInfo& GetIncetance(ID3D11Device* pDeviceIn);

public:
	std::vector<FilterVertex> vFilterVertices;
	std::vector<UINT> vIndices;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIndexBuffer;

public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpFilterVS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpFilterInputLayout;

public:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> cpPSSamplerState;
};

class FilterInterface
{
public:
	FilterInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn
	);
	virtual ~FilterInterface() { };

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

protected:
	D3D11_VIEWPORT sScreenViewport;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpOutputTexture2D;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cpOutputRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpOutputSRV;

public:
	inline ID3D11Texture2D* GetOutputTexture2D() { return cpOutputTexture2D.Get(); }
	inline ID3D11RenderTargetView* GetOutputRTV() { return cpOutputRTV.Get(); }
	inline ID3D11ShaderResourceView* GetOutputSRV() { return cpOutputSRV.Get(); }
	inline ID3D11Texture2D** GetAddressOfOutputTexture2D() { return cpOutputTexture2D.GetAddressOf(); }
	inline ID3D11RenderTargetView** GetAddressOfOutputRTV() { return cpOutputRTV.GetAddressOf(); }
	inline ID3D11ShaderResourceView** GetAddressOfOutputSRV() { return cpOutputSRV.GetAddressOf(); }

public:
	virtual void Update() = 0;
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) = 0;
};

