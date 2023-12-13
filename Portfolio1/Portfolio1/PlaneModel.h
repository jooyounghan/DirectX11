#pragma once
#include "PBRModel.h"

class PlaneModel : public PBRModel
{
public:
	PlaneModel(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const float& fCenterXIn, 	const float& fCenterYIn, const float& fCenterZIn,
		const float& fWidthIn, const float& fHeightIn,
		const float& fWrapWidthIn, const float& fWrapHeightIn
	);
	~PlaneModel();
};

