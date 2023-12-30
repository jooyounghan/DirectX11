#pragma once
#include "AShadowDrawer.h"

class PointLightShadowDrawer : public AShadowDrawer
{
public:
	PointLightShadowDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~PointLightShadowDrawer();

public:
	virtual void Draw(
		LightInterface* pLightInterface,
		const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
	) override;
};


