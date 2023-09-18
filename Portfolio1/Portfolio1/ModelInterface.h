#pragma once
#include "TransformProperties.h"
#include "ModelID.h"
#include "TextureSet.h"

struct Vector
{
	float x, y, z;
	float w = 0.f;
};

typedef Vector PositionVector;

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
public:
	ModelInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen
	);
	~ModelInterface() {};

public:
	void Update();
	void Render();
	void RenderOutline();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

public:
	TransformProperties						sTransformationProperties;
	TransformationBufferData				sTransformationBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTransformationDataBuffer;

public:
	ModelID		modelID;
	TextureSet	sTextures;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;
};

