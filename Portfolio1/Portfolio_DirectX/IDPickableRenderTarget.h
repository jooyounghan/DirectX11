#pragma once
#include "AFilter.h"
#include "RenderTarget.h"

class IDPickableRenderTarget : public AFilter, public RenderTarget
{
public:
	IDPickableRenderTarget(
		const UINT& uiWidthIn, 
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn
	);
	virtual ~IDPickableRenderTarget();

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

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override;
	virtual void SetUAVBarrier() override;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

