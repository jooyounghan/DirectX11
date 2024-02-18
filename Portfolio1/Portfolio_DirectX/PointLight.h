#pragma once
#include "ILight.h"
#include "IAngleAdjustable.h"
#include "ViewableDepthOnly.h"

enum EPointDirections : size_t
{
	PointXDirection,
	PointNegXDirection,
	PointYDirection,
	PointNegYDirection,
	PointZDirection,
	PointNegZDirection,
	PointDirectionNum
};

class PointLight : public ILight
{
	friend class LightRenderer;
	friend class ModelRenderer;
	friend class LightManipulator;

public:
	PointLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fLightRColorIn,
		const float& fLightGColorIn,
		const float& fLightBColorIn,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const float& fLightPowerIn
	);
	virtual ~PointLight();

private:
	static size_t ullPointLightCnt;
	size_t ullPointLightId;

protected:
	ViewableDepthOnly viewablesDirections[6];

public:
	virtual size_t GetLightID();
	virtual void UpdateLight() override;

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) override;

public:
	virtual void AcceptSettingForDirectLighting(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptResetingForDirectLighting(class ModelRenderer* pModelRenderer) override;

public:
	virtual void AcceptLightList(class LightManipulator* pLightManipulator) override;
	virtual void AcceptLightSetting(class LightManipulator* pLightManipulator) override;
};