#pragma once
#include "ID3D11Helper.h"
#include "StructVar.h"

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
		IN class ImageContainer* pImageContainer
	);

public:
	ComPtr<ID3D11Texture2D>				AOTexture2D;
	ComPtr<ID3D11ShaderResourceView>	AOSRV;
	ComPtr<ID3D11Texture2D>				DiffuseTexture2D;
	ComPtr<ID3D11ShaderResourceView>	DiffuseSRV;
	ComPtr<ID3D11Texture2D>				HeightTexture2D;
	ComPtr<ID3D11ShaderResourceView>	HeightSRV;
	ComPtr<ID3D11Texture2D>				NormalTexture2D;
	ComPtr<ID3D11ShaderResourceView>	NormalSRV;
	ComPtr<ID3D11Texture2D>				ReflectTexture2D;
	ComPtr<ID3D11ShaderResourceView>	ReflectSRV;
};

class IModel
{
public:
	IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~IModel() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t				ui32IndexCount;

public:
	ComPtr<ID3D11Buffer>	cpVertexBuffer;

public:
	ModelTransform			sModelTransformation;
	ComPtr<ID3D11Buffer>	cpModelMatrixBuffer;

public:
	TextureSet				STextures;

protected:
	ComPtr<ID3D11Device>& cpDevice;
	ComPtr<ID3D11DeviceContext>& cpDeviceContext;
};