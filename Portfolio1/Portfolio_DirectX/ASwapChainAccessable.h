#pragma once

class ASwapChainAccessable
{
public:
	ASwapChainAccessable();
	~ASwapChainAccessable();

public:
	ID3D11Texture2D* p_back_buffer;

public:
	virtual void SetAsBackBufferAddress();
	virtual void ResetBackBufferAddress();
};

