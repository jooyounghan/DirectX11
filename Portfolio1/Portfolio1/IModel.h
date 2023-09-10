#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>
#include <atomic>
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

struct ModelID
{
	unsigned int ullModelID;
	unsigned int ullDummy[3];
};

class IModel
{
public:
	IModel(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~IModel() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t				ui32IndexCount;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;

public:
	ModelTransform			sModelTransformation;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelMatrixBuffer;

public:
	TextureSet				STextures;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelIDBuffer;
	ModelID ullModelID;

protected:
	static unsigned int ullCurrentModelID;
	static std::mutex mtxId;

protected:
	void SetModelID();
};