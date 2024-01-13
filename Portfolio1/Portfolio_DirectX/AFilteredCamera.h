#pragma once
#include "ICamera.h"
#include "IFilter.h"
#include <vector>

class AFilteredCamera : public ICamera
{
public:
	AFilteredCamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		IDXGISwapChain* pSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~AFilteredCamera();

protected:
	std::vector<IFilter*> pFilters;

public:
	void Filter();

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
	virtual void UpdateCamera() = 0;
};

