#pragma once

class SwapChainAccessable
{
public:
	SwapChainAccessable();
	~SwapChainAccessable();

public:
	struct ID3D11Texture2D* p_back_buffer;

public:
	virtual void SetAsBackBufferAddress();
	virtual void ResetBackBufferAddress();
};

