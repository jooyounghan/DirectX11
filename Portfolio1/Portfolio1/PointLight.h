#pragma once
#include "LightInterface.h"

struct PointLightSet
{
	DirectX::XMVECTOR	xmvLocation;
	DirectX::XMVECTOR	xmvLightColor;
	float				fFallOffStart;
	float				fFallOffEnd;
	float				fLightPower;
	float				fDummy;
};


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
	PointLight(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn,
		const DirectX::XMVECTOR& xmvLocationIn,
		const DirectX::XMVECTOR& xmvLightColorIn,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const float& fLightPowerIn
	);
	~PointLight();

protected:
	static DirectX::XMVECTOR xmvDirectDefault[6];
	static DirectX::XMVECTOR xmvUpDefault[6];

public:
	PointLightSet sPointLightSet;
	inline PointLightSet* GetLightDataPtr() { return &sPointLightSet; }

protected:
	struct
	{
		DirectX::XMMATRIX	xmmViewProj;
		DirectX::XMMATRIX	xmmViewProjInv;
	} sPointLightViewProjData[PointViewProjNum];

	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPointLightViewProjDataBuffer[PointViewProjNum];

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpShadowMapTexture[PointViewProjNum];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpShadowMapSRV[PointViewProjNum];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpShadowMapDSV[PointViewProjNum];

public:
	virtual void Update();

private:
	virtual void SetConstantBuffers();;

public:
	void SetConstantBuffers(const size_t& uiViewProjIdx);
	virtual void ResetConstantBuffers();


private:
	virtual void SetShaderResources();;
	
public:
	void SetShaderResources(const size_t& uiViewProjIdx);
	virtual void ResetShaderResources();
};

