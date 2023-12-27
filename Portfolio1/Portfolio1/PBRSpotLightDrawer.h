#pragma once
#include "APBRDirectLightDrawer.h"

class PBRSpotLightDrawer : public APBRDirectLightDrawer
{
public:
	PBRSpotLightDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~PBRSpotLightDrawer();
};
