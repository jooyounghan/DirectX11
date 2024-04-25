#pragma once
#include "AFilter.h"

class ADualInputFilter : public AFilter
{
public:
	ADualInputFilter(
		UINT uiWidthIn,
		UINT uiHeightIn,
		UINT uiArraySizeIn,
		UINT uiNumQualityLevelsIn,
		UINT uiBindFlagIn,
		UINT uiCPUAccessIn,
		UINT uiMiscFlagIn,
		D3D11_USAGE eUsageIn,
		DXGI_FORMAT eFormatIn
	);
	virtual ~ADualInputFilter();

protected:
	ID3D11ShaderResourceView** ppPresetInputSRV;

public:
	inline void PresetInput(ID3D11ShaderResourceView** ppPresetInputSRVIn) { ppPresetInputSRV = ppPresetInputSRVIn; }

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) = 0;
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) = 0 ;
};

