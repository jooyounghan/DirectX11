#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

enum TextureType : unsigned int
{
	TEXTURE_AO,
	TEXTURE_DIFFUSE,
	TEXTURE_HEIGHT,
	TEXTURE_NORMAL,
	TEXTURE_REFLECT,
};

class TextureSet
{
public:
	TextureSet() {};
	~TextureSet() {};

public:
	void CreateTextureAndSRV(
		IN TextureType eTextureType,
		IN ID3D11Device* pDevice,
		IN ID3D11DeviceContext* pDeviceContext,
		IN class ImageContainer* pImageContainer
	);

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				AOTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	AOSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				DiffuseTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	DiffuseSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				HeightTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	HeightSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				NormalTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	NormalSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				ReflectTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ReflectSRV;
};