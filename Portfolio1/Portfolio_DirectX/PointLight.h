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

class PointLight : public ILight, public IAngleAdjustable
{
	friend class LightRenderer;
	friend class ModelRenderer;

public:
	PointLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn
	);
	virtual ~PointLight();

protected:
	ViewableDepthOnly viewablesDirections[6];

public:
	virtual void UpdateLight() override;

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) override;

public:
	virtual void AcceptSettingForDirectLighting(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptResetingForDirectLighting(class ModelRenderer* pModelRenderer) override;

};