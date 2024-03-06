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
	static D3D11_VIEWPORT nullViewPort;

public:
	inline static const D3D11_VIEWPORT& GetNullViewPort() { return nullViewPort; };

protected:
	struct 
	{
		DirectX::XMMATRIX xmmViewProjMat;
		DirectX::XMMATRIX xmmInvViewProjMat;
	} sViewProjs;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpViewProjBuffer;

public:
	inline ID3D11Buffer* const* GetViewProjBuffer() { return cpViewProjBuffer.GetAddressOf(); }

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual void UpdateView();
};

