#pragma once
#include "ILight.h"
#include "IAngleAdjustable.h"
#include "ViewableDepthOnly.h"

enum EPointDirections : size_t
{
	PointXDirection,
	PointNegXDirection,
	PointYDirection,
	PointNegYDirection,
	PointZDirection,
	PointNegZDirection,
	PointDirectionNum
};

class PointLight : public ILight, public IAngleAdjustable
{
public:
	PointLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn
	);
	virtual ~PointLight();

protected:
	ViewableDepthOnly viewablesDirections[6];

public:
	inline ID3D11DepthStencilView* GetDSV(const EPointDirections& eDirection) { return viewablesDirections[eDirection].cpDSV.Get(); }
	inline void ClearDSV(const EPointDirections& eDirection) { return viewablesDirections[eDirection].ClearDSV(); }
	inline const D3D11_VIEWPORT& GetViewPort(const EPointDirections& eDirection) { return viewablesDirections[eDirection].sViewPort; }
	inline ID3D11Buffer** GetAddressOfViewProj(const EPointDirections& eDirection) { return viewablesDirections[eDirection].cpViewProjBuffer.GetAddressOf(); }

public:
	virtual void UpdateLight() override;
	virtual void UpdateView();
};