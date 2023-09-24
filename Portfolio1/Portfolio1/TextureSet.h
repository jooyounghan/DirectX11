#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

enum TextureType
{
	TEXTURE_AO,
	TEXTURE_DIFFUSE,
	TEXTURE_HEIGHT,
	TEXTURE_NORMAL,
	TEXTURE_REFLECT,
	TEXTURE_NUM
};

class TextureSet
{
public:
	TextureSet();
	~TextureSet();

public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResources[TEXTURE_NUM];

public:
	void SetTextureToShaderResource(TextureType textureType, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpTextureShaderResource);

public:
	inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV(TextureType textureType) { return ShaderResources[textureType]; };
};