#pragma once
#include "TransformProperties.h"
#include "ModelID.h"
#include "TextureSet.h"

struct PositionVector
{
	float x;
	float y;
	float z;
	float w = 1.f;
};

struct Vector
{
	float x;
	float y;
	float z;
	float w = 0.f;
};

struct TextureCoord
{
	float x;
	float y;
};

struct Vertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
	Vector			sNorVec;
	Vector			sTangVec;
};

class ModelInterface
{
template<typename Drawer>
friend class Canvas;

public:
	ModelInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	~ModelInterface() {};

public:
	void Update();

protected:
	void Render();

protected:
	void SetIAProperties();
	void SetVSConstantBuffers();
	void SetHSConstantBuffers();
	void SetDSConstantBuffers();
	void SetPSConstantBuffers();

protected:
	void SetVSShaderResources();
	void SetHSShaderResources();
	void SetDSShaderResources();
	void SetPSShaderResources();

public:
	void ScaleUp(const float& x, const float& y, const float& z);

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

public:
	TransformProperties						sTransformationProperties;
	TransformationBufferData				sTransformationBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTransformationDataBuffer;

public:
	TextureSet	sTextureSet;
	ModelID		modelID;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;
};

