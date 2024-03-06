#pragma once
#include "ShaderResource.h"

class CubeDepthStencilView : public ShaderResource
{
public:
	CubeDepthStencilView(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn
	);
	virtual ~CubeDepthStencilView();

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cpDSVs[6];
};

