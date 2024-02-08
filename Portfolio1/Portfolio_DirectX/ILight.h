#pragma once
#include "IMovable.h"
#include <unordered_map>

class ILight : virtual public IMovable
{
protected:
	static constexpr float gLightFovDeg = 90.f;
	static constexpr UINT gShadowMapWidth = 1000;
	static constexpr UINT gShadowMapHeight = 1000;
	static constexpr float gDefaultFallOffStart = 0.001f;
	static constexpr float gDefaultFallOffEnd = 10.f;

public:
	ILight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos
	);
	virtual ~ILight();

public:
	struct
	{
		float fLightColor[3];
		float fFallOffStart;
		float fFallOffEnd;
		float fLightPower;
		float fDummy[2];
	} sBaseLightData;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightBuffer;

public:
	virtual void UpdateLight() = 0;

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) = 0;

public:
	virtual void AcceptSettingForDirectLighting(class ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptResetingForDirectLighting(class ModelRenderer* pModelRenderer) = 0;

};