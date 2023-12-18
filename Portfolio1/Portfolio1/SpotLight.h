#pragma once

#include "LightInterface.h"

class SpotLight : public LightInterface
{
public:
	SpotLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn);
	~SpotLight();

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
};

