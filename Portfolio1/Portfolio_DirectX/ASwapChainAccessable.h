#pragma once
#include "IRenderTarget.h"

class ASwapChainAccessable : public ITexture2D
{
public:
	ASwapChainAccessable();
	virtual ~ASwapChainAccessable();

public:
	bool IsSwapChainAccesssed;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpFormatChangedTexture;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	cpFormatChangedUAV;

public:
	void SetAsSwapChainBackBuffer();
	void ResetFromSwapChain();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

