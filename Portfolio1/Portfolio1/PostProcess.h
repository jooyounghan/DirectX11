#pragma once
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

class PostProcess
{
public:
	PostProcess(
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpStartTextureIn,
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpBackBufferTextureIn,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& cpBackBufferRTVIn,
	);
	~PostProcess();

public:
};

