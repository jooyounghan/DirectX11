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
	virtual void UpdateLight(const std::unordered_map<uint32_t, AStaticMesh*>& pModels) override;
};