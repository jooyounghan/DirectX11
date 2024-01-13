#pragma once
#include "IRenderTarget.h"

class PickableIDRenderTarget : public IRenderTarget
{
public:
	PickableIDRenderTarget(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiNumQualityLevelsIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn
	);
	virtual ~PickableIDRenderTarget();

protected:
	ID3D11DeviceContext* pDeviceContext;

public:
	class StageShaderResource* pMSToSSSRV;
	class StageShaderResource* pStageSRV;

public:
	WORD GetPickedID(const UINT& uiMouseXIn, const UINT& uiMouseYIn);

public:
	virtual void ClearRTV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

