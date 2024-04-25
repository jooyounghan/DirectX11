#pragma once
#include "ADualInputFilter.h"

class BloomFilter : public ADualInputFilter
{
	
	friend class CameraManipulator;

public:
	BloomFilter(
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
	virtual ~BloomFilter();

protected:
	class InputMergeComputeShader* pBloomCS;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) override;
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override;
};

