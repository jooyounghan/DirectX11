#pragma once
#include "IRectangle.h"
#include <d3d11.h>
#include <d3dcompiler.h>

class IDepthStencil : virtual public IRectangle
{
public:
	IDepthStencil(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	virtual ~IDepthStencil();

public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cpDSV;

public:
	virtual void ClearDSV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

