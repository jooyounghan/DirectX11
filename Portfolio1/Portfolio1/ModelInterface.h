#pragma once
#include "TransformProperties.h"
#include "ModelID.h"
#include "ModelStruct.h"

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
	~ModelInterface();

public:
	void Update();

protected:
	void Render();

protected:
	void SetIAProperties();
	void SetVSConstantBuffers();
	void SetHSConstantBuffers();
	void SetDSConstantBuffers();
	void SetGSConstantBuffers();
	void SetPSConstantBuffers();

protected:
	void SetVSShaderResources();
	void SetHSShaderResources();
	void SetDSShaderResources();
	void SetGSShaderResources();
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
	class ModelTextureSet*		pModelTextureSet;
	ModelID						modelID;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureFlagBuffer;
	struct
	{
		bool bAODummy[3];
		bool bIsAOTexture;
		bool bDiffuseDummy[3];
		bool bIsDiffuseTexture;
		bool bReflectDummy[3];
		bool bIsReflectTexture;
		bool bNormalDummy[3];
		bool bIsNormalTexture;
	} sPSTextureFlags;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;
};

