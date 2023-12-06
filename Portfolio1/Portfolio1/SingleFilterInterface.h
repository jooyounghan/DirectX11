#pragma once
#include "FilterInterface.h"

class SingleFilterInterface : public FilterInterface
{
public:
	SingleFilterInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~SingleFilterInterface();

protected:
	struct
	{
		float fDeltaWidth;
		float fDeltaHeight;
		float fDummy[2];
	} sViewportInfoData;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpViewportConstantBuffer;

public:
	virtual void Update() override final {};

public:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) = 0;
};

