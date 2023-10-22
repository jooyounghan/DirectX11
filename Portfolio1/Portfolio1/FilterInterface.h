#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>

#include "ModelStruct.h"

struct FilterVertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
};

class FilterInfo
{
private:
	FilterInfo(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	FilterInfo(const FilterInfo& ref){}
	FilterInfo& operator=(const FilterInfo& ref) {}
	~FilterInfo() {}

public:
	static FilterInfo& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	std::vector<FilterVertex> vFilterVertices;
	std::vector<unsigned int> vIndices;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIndexBuffer;

public:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> cpPSSamplerState;
};

class FilterInterface
{
public:
	FilterInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn
	);
	virtual ~FilterInterface() { };

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

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
	virtual void CreateOutputResource(DXGI_FORMAT eFormat) = 0;
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) = 0;
};

