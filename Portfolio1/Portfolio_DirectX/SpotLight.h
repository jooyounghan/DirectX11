#pragma once
#include "ILight.h"
#include "ViewableDepthOnly.h"

class SpotLight : public ILight, public ViewableDepthOnly
{
public:
	SpotLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const float& fLightRColorIn,
		const float& fLightGColorIn,
		const float& fLightBColorIn,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const float& fLightPowerIn,
		const float& fSpotPowerIn
	);
	virtual ~SpotLight();

private:
	static size_t ullSpotLightCnt;
	size_t ullSpotLightId;

protected:
	struct
	{
		float fSpotPower;
		float fDummy[3];
	} sSpotLightData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpSpotLightBuffer;

public:
	inline float* GetSpotPower() { return &sSpotLightData.fSpotPower; }
	inline ID3D11Buffer* const* GetSpotLightBuffer() { return cpSpotLightBuffer.GetAddressOf(); }

public:
	virtual size_t GetLightID();
	virtual void UpdateLight() override;

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) override;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) {};

public:
	virtual void AcceptSettingForDirectLighting(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptResetingForDirectLighting(class ModelRenderer* pModelRenderer) override;

public:
	virtual void AcceptLightList(class LightManipulator* pLightManipulator) override;
	virtual void AcceptLightSetting(class LightManipulator* pLightManipulator) override;
};