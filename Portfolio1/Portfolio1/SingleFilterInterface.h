#pragma once
#include "FilterInterface.h"

class SingleFilterInterface : public FilterInterface
{
public:
	SingleFilterInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
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

