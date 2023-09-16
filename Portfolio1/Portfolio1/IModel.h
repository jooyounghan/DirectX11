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

class ModelID
{
	friend class IModel;

public:
	ModelID();
	ModelID& operator= (const ModelID& modelIDRight);
	friend bool operator==(const ModelID& modelID1, const ModelID& modelID2);

public:
	static ModelID ConvertR8G8B8A8ToModelID(const unsigned int& RGBA);

protected:
	static ModelID ullCurrentModelID;
	static ModelID IssueModelID();
	static std::mutex mtxId;

protected:
	template<typename ...Args>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper, Args & ...IdUppers);
	template<>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper);


public:
	unsigned int ucModelID[3];
	unsigned int ucModelIDStd = 0xFF;
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

public:
	ModelID ullModelID;


protected:
	void SetModelID();
};
