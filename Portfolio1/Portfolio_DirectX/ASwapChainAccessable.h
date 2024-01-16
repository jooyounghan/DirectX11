#pragma once
#include "ITexture2D.h"

class ASwapChainAccessable : public ITexture2D
{
public:
	ASwapChainAccessable();
	virtual ~ASwapChainAccessable();

public:
	bool IsSwapChainAccesssed;

public:
	void SetAsSwapChainBackBuffer();
	void ResetFromSwapChain();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

