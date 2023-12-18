#pragma once
#include "LightInterface.h"

enum PointLightViewProj : size_t
{
	XViewProj,
	XNegViewProj,
	YViewProj,
	YNegViewProj,
	ZViewProj,
	ZNegViewProj,
	PointViewProjNum
};

class PointLight : public LightInterface
{
public:
	PointLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn);
	~PointLight();

public:
	static DirectX::XMVECTOR xmvDirectDefault[6];
	static DirectX::XMVECTOR xmvUpDefault[6];

protected:
	struct
	{
		DirectX::XMMATRIX	xmmViewProj[PointViewProjNum];
		DirectX::XMMATRIX	xmmViewProjInv[PointViewProjNum];
	} sPointLightViewProjData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPointLightViewProjDataBuffer;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpShadowMapTexture[PointViewProjNum];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpShadowMapSRV[PointViewProjNum];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpShadowMapDSV[PointViewProjNum];

public:
	virtual void Update();

public:
	virtual void SetConstantBuffers();
	virtual void ResetConstantBuffers();

public:
	virtual void SetShaderResources();
	virtual void ResetShaderResources();
};

