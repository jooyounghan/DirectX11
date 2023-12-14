#pragma once
#include "LightInterface.h"

class PointLight : public LightInterface
{
public:
	PointLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn);
	~PointLight();

public:
	struct
	{
		float				fLocation[3];
		float				fDirection[3];
		DirectX::XMMATRIX	xmmLightViewProj;
		DirectX::XMMATRIX	xmmLightViewProjInv;
		float				fFallOffStart;
		float				fFallOffEnd;
	} sPointLightData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPointLightDataBuffer;

public:
	virtual void Update();

public:
	virtual void SetConstantBuffers();
	virtual void ResetConstantBuffers();

public:
	virtual void SetShaderResources();
	virtual void ResetShaderResources();
};

