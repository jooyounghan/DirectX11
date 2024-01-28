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
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn
	);
	virtual ~PointLight();

protected:
	ViewableDepthOnly viewablesDirections[6];

public:
	virtual void UpdateLight(const std::vector<class AStaticMesh*>& pModels) override;
};