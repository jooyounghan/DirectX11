#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <string>

enum TextureType
{
	TEXTURE_AO,
	TEXTURE_DIFFUSE,
	TEXTURE_HEIGHT,
	TEXTURE_NORMAL,
	TEXTURE_REFLECT,
	TEXTURE_NUM
};

struct Texture
{
	std::wstring										TextureName;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	TextureSRV;
};


class TextureSet
{
public:
	TextureSet();
	~TextureSet();

public:
	Texture Textures[TEXTURE_NUM];

public:
	void SetTextureToShaderResource(TextureType textureType, Texture* pTexture);

public:
	inline std::wstring& GetTextureName(TextureType textureType) { return Textures[textureType].TextureName; }
	inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV(TextureType textureType) { return Textures[textureType].TextureSRV; };
};