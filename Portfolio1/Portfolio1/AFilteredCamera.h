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
		UINT uiWidthIn, UINT uiHeightIn,
		UINT uiArraySizeIn,
		UINT uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	~AFilteredCamera();

protected:
	std::vector<IFilter*> pFilters;

public:
	void Filter();

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize() = 0;
	virtual void UpdateCamera() = 0;
};

