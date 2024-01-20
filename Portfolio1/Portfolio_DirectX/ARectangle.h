#pragma once
#include <windows.h>
#include <wrl/client.h>

class ARectangle
{
public:
	ARectangle();
	ARectangle(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	~ARectangle();

public:
	UINT uiWidth;
	UINT uiHeight;

public:
	void SetRectangle(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

