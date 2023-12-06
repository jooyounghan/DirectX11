#pragma once
#include "FilterInterface.h"

class BlendFilterInterface : public FilterInterface
{
public:
	BlendFilterInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BlendFilterInterface();

protected:
	struct
	{
		float fBlendStrength;
		float fExposure;
		float fGamma;
		float fDummy;
	} sBlendConstantData;

public:
	inline void SetBlendStrength(const float& fBlendStrengthIn) { sBlendConstantData.fBlendStrength = fBlendStrengthIn; };
	inline void SetExposure(const float& fExposureIn) { sBlendConstantData.fExposure = fExposureIn; };
	inline void SetGamma(const float& fGammaIn) { sBlendConstantData.fGamma = fGammaIn; };

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBlendConstantBuffer;

public:
	virtual void Update() override final;

public:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInput1SRV, ID3D11ShaderResourceView** ppInput2SRV) = 0;

private:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) override final {};
};
