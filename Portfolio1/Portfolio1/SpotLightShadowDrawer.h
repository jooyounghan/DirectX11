#pragma once
#include "AShadowDrawer.h"

class SpotLightShadowDrawer : public AShadowDrawer
{
public:
	SpotLightShadowDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~SpotLightShadowDrawer();

public:
	virtual void Draw(
		LightInterface* pLightInterface,
		const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
	) override;
};

