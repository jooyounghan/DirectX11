#include "ADualInputFilter.h"

ADualInputFilter::ADualInputFilter(
	UINT uiWidthIn, UINT uiHeightIn, 
	UINT uiArraySizeIn, 
	UINT uiNumQualityLevelsIn, 
	UINT uiBindFlagIn, 
	UINT uiCPUAccessIn, 
	UINT uiMiscFlagIn, 
	D3D11_USAGE eUsageIn, 
	DXGI_FORMAT eFormatIn
)
	: AFilter(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn,
		eFormatIn,
		32, 32, 1
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
}

ADualInputFilter::~ADualInputFilter()
{
}
