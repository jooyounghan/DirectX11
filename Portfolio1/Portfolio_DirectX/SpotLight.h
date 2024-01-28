#pragma once
#include "ILight.h"
#include "ViewableDepthOnly.h"

class SpotLight : public ILight, protected ViewableDepthOnly
{
public:
	SpotLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn
	);
	virtual ~SpotLight();

public:
	struct
	{
		float fSpotPower;
		float fDummy[3];
	} sSpotLightData;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpSpotLightBuffer;

public:
	virtual void UpdateLight(const std::vector<class AStaticMesh*>& pModels) override;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) {};
};