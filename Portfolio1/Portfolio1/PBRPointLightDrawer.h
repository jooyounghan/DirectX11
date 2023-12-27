#pragma once
#include "APBRDirectLightDrawer.h"

class PBRPointLightDrawer : public APBRDirectLightDrawer
{
public:
	PBRPointLightDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~PBRPointLightDrawer();
};
