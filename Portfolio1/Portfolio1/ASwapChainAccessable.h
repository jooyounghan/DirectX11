#pragma once
#include "ITexture2D.h"

class ASwapChainAccessable : public ITexture2D
{
public:
	ASwapChainAccessable(
		ID3D11Device*	pDeviceIn,
		IDXGISwapChain* pSwapChainIn
	);
	~ASwapChainAccessable();

protected:
	IDXGISwapChain* pSwapChain;

public:
	bool IsSwapChainAccesssed;

public:
	void SetAsSwapChainBackBuffer();
	void ResetFromSwapChain();

public:
	virtual void Resize() = 0;
};

