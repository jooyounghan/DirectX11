#pragma once
#include "AFilter.h"

class BlurFilter : public AFilter
{
	friend class CameraManipulator;

public:
	BlurFilter(
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
	virtual ~BlurFilter();

protected:
	class BlurComputeShader* pBlurCS;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator);
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override;
};

