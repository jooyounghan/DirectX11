#pragma once
#include "ModelInterface.h"

class SphereModel : public ModelInterface
{
public:
	SphereModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const unsigned short& usLevel = 8
	);
	~SphereModel();
};
