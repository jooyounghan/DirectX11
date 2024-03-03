#pragma once
#include "IMovable.h"
#include <unordered_map>

class ILight : virtual public IMovable
{
public:
	static constexpr float gLightFovDeg = 90.f;
	static constexpr float gLightNearZ = 0.001f;
	static constexpr UINT gShadowMapWidth = 1000;
	static constexpr UINT gShadowMapHeight = 1000;

public:
	ILight(
		const float& fXPosIn,
		const float& fYPosIn,
		const float& fZPosIn,
		const float& fLightRColorIn,
		const float& fLightGColorIn,
		const float& fLightBColorIn,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const float& fLightPowerIn
	);
	virtual ~ILight();

protected:
	struct
	{
		float fLightColor[3];
		float fFallOffStart;
		float fFallOffEnd;
		float fLightPower;
		float fDummy[2];
	} sBaseLightData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightBuffer;

public:
	inline float* GetLightColor() { return sBaseLightData.fLightColor; }
	inline float* GetLightFallOffStart() { return &sBaseLightData.fFallOffStart; }
	inline float* GetLightFallOffEnd() { return &sBaseLightData.fFallOffEnd; }
	inline float* GetLightPower() { return &sBaseLightData.fLightPower; }
	inline ID3D11Buffer* const* GetBaseLightBuffer() { return cpBaseLightBuffer.GetAddressOf(); }

public:
	virtual size_t GetLightID() = 0;
	virtual void UpdateLight() = 0;

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) = 0;

public:
	virtual void AcceptSettingForDirectLighting(class ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptResetingForDirectLighting(class ModelRenderer* pModelRenderer) = 0;

public:
	virtual void AcceptLightList(class LightManipulator* pLightManipulator) = 0;
	virtual void AcceptLightSetting(class LightManipulator* pLightManipulator) = 0;
};