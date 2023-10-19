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

class FilterInterface
{
public:
	FilterInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpInputTexture2DIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	virtual ~FilterInterface() = 0;

public:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpInputTexture2D;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpOutputTexture2D;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cpOutputRTV;

public:
	UINT uiWidth;
	UINT uiHeight;

public:
	std::vector<FilterVertex> vFilterVertices;
	std::vector<unsigned int> vIndices;

public:
	virtual void CreateOutputResource() = 0;
	virtual void StartFilter() = 0;
};

