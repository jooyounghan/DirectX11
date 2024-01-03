#pragma once

#include "LightInterface.h"

struct SpotLightSet
{
	DirectX::XMVECTOR	xmvLocation;
	DirectX::XMVECTOR	xmvAngles;
	DirectX::XMVECTOR	xmvLightColor;
	float				fFallOffStart;
	float				fFallOffEnd;
	float				fLightPower;
	float				fSpotPower;
};


class SpotLight : public LightInterface
{
public:
	SpotLight(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn,
		const DirectX::XMVECTOR& xmvLocationIn,
		const DirectX::XMVECTOR& xmvAnglesIn,
		const DirectX::XMVECTOR& xmvLightColorIn,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const float& fLightPowerIn,
		const float& fSpotPowerIn
	);
	~SpotLight();

public:
	SpotLightSet	sSpotLightSet;
	inline SpotLightSet* GetLightDataPtr() { return &sSpotLightSet; }

protected:
	struct
	{
		DirectX::XMMATRIX	xmmViewProj;
		DirectX::XMMATRIX	xmmViewProjInv;
	} sSpotLightViewProjData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cpSpotLightViewProjDataBuffer;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpShadowMapTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpShadowMapSRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpShadowMapDSV;

public:
	virtual void Update();

public:
	virtual void SetConstantBuffers();
	virtual void ResetConstantBuffers();

public:
	virtual void SetShaderResources();
	virtual void ResetShaderResources();

public:
	void OMSetRenderTarget();

public:
	void WipeOut();
};

