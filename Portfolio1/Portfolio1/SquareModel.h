#pragma once
#include "PBRModel.h"

class SquareModel : public PBRModel
{
public:
	SquareModel(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen
	);
	~SquareModel();
};

