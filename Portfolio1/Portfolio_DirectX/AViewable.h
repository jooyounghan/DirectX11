#pragma once
#include "IMovable.h"
#include "IAngleAdjustable.h"

class AViewable : public IMovable, public IAngleAdjustable
{
public:
	AViewable(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fWidthIn,
		const float& fHeightIn,
		const float& fFovRadianIn, 
		const float& fNearZIn, 
		const float& fFarZIn
	);
	virtual ~AViewable();

public:
	D3D11_VIEWPORT sViewPort;
	float fFovRadian;
	float fNearZ;
	float fFarZ;

protected:
	struct 
	{
		DirectX::XMMATRIX			xmmViewProjMat;
		DirectX::XMMATRIX			xmmInvViewProjMat;
	} sViewProjs;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpViewProjBuffer;

protected:
	void UpdateViewProj();

public:
	virtual void Update(const float& fDelta) = 0;
};

