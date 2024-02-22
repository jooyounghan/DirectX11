#pragma once
#include "ShaderResource.h"

class CubeRenderTargets : public ShaderResource
{
public:
	CubeRenderTargets(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn
	);
	virtual ~CubeRenderTargets();

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cpDSVs[6];

public:
	ID3D11DepthStencilView* GetDSVAddress(const size_t& idx);
};

