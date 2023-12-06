#pragma once
#include "PBRModel.h"

class SphereModel : public PBRModel
{
public:
	SphereModel(
		ID3D11Device* cpDeviceIn,
		ID3D11DeviceContext* cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const unsigned short& usLevel = 8
	);
	~SphereModel();
};