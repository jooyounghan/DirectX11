#pragma once
#include <wrl/client.h>

class IRectangle
{
public:
	IRectangle(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn
	);
	~IRectangle();

public:
	UINT uiWidth;
	UINT uiHeight;
	UINT uiArraySize;
	UINT uiNumQualityLevels;
};

