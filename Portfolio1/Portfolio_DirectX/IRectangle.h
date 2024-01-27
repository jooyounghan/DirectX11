#pragma once
#include <windows.h>
#include <wrl/client.h>

class IRectangle
{
public:
	IRectangle(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	~IRectangle();

public:
	UINT uiWidth;
	UINT uiHeight;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

