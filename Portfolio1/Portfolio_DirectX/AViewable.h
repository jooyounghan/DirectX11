#pragma once
#include "IMovable.h"

class AViewable : public IMovable
{
public:
	AViewable(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn,
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
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpViewProjBuffer;

protected:
	void UpdateViewProj();

public:
	virtual void Update(const float& fDelta) = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

