#pragma once
#include "IRectangle.h"
#include "IMovable.h"
#include "IAngleAdjustable.h"

#include <tuple>

class Viewable : virtual public IRectangle, virtual public IMovable, virtual public IAngleAdjustable
{
public:
	Viewable(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const float& fWidthIn,
		const float& fHeightIn,
		const float& fFovDegreeIn, 
		const float& fNearZIn, 
		const float& fFarZIn
	);
	virtual ~Viewable();

public:
	D3D11_VIEWPORT sViewPort;
	float fFovDegree;
	float fNearZ;
	float fFarZ;

protected:
	struct 
	{
		DirectX::XMMATRIX xmmViewProjMat;
		DirectX::XMMATRIX xmmInvViewProjMat;
	} sViewProjs;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpViewProjBuffer;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	virtual void UpdateView();
};

