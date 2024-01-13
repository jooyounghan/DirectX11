#include "IRectangle.h"

IRectangle::IRectangle(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn
)
	: uiWidth(uiWidthIn), uiHeight(uiHeightIn), uiArraySize(uiArraySizeIn), uiNumQualityLevels(uiNumQualityLevelsIn)
{
}

IRectangle::~IRectangle()
{
}
