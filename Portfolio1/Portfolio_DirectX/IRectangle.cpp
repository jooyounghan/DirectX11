#include "IRectangle.h"

IRectangle::IRectangle()
	: uiWidth(0), uiHeight(0), uiArraySize(0), uiNumQualityLevels(0)
{
}

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
