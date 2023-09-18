#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>
#include <atomic>

#include "StructVar.h"
#include "ModelID.h"
#include "TextureSet.h"

class IModel
{
public:
	IModel(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~IModel() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderOutline() = 0;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t				ui32IndexCount;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;

public:
	ModelTransform			sModelTransformation;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelMatrixBuffer;

public:
	TextureSet				sTextures;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

public:
	ModelID modelID;

protected:
	void SetModelID();
};
