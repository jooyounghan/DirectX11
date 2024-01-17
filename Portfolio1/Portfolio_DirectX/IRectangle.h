#pragma once
#include <windows.h>
#include <wrl/client.h>

class IRectangle
{
public:
	IRectangle();
	IRectangle(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	~IRectangle();

public:
	UINT uiWidth;
	UINT uiHeight;
};

