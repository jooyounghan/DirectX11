#pragma once
#include "IFilter.h"
#include "ARenderTarget.h"

class IDPickableRenderTarget : public IFilter, public ARenderTarget
{
public:
	IDPickableRenderTarget(
		const UINT& uiWidthIn, 
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn
	);
	~IDPickableRenderTarget();

public:

	Microsoft::WRL::ComPtr<ID3D11Texture2D>	cpPickedIDResult;

protected:
	struct
	{
		uint32_t uiMouseXNdc;
		uint32_t uiMouseYNdc;
		uint32_t fDummy[2];
	} sMousePosNdc;

public:
	virtual void SetMousePos(const int& iMouseX, const int& iMouseY);

public:
	virtual uint32_t	GetPickedID() = 0;
	virtual void		Apply(ID3D11ShaderResourceView** ppInputSRV) = 0;
	virtual void		SetUAVBarrier() = 0;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

