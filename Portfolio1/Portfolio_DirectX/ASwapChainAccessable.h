#pragma once
#include "ITexture2D.h"

class ASwapChainAccessable : public ITexture2D
{
public:
	ASwapChainAccessable(
		ID3D11Device*	pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		IDXGISwapChain* pSwapChainIn
	);
	virtual ~ASwapChainAccessable();

protected:
	IDXGISwapChain* pSwapChain;

public:
	bool IsSwapChainAccesssed;

public:
	void SetAsSwapChainBackBuffer();
	void ResetFromSwapChain();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

