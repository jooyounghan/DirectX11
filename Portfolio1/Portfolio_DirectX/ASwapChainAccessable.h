#pragma once
#include "ARenderTarget.h"

class ASwapChainAccessable : public Texture2D
{
public:
	ASwapChainAccessable();
	virtual ~ASwapChainAccessable();

public:
	bool IsSwapChainAccesssed;

public:
	virtual void SetAsSwapChainBackBuffer();
	virtual void ReleaseAndGetAddressOfFromSwapChain();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

