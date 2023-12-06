#pragma once
#include "SingleFilterInterface.h"

class BloomShader
{
private:
	BloomShader(ID3D11Device* pDeviceIn);
	BloomShader(const BloomShader& ref) {}
	BloomShader& operator=(const BloomShader& ref) {}
	~BloomShader() {}

public:
	static BloomShader& GetIncetance(ID3D11Device* pDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBloomPS;
};

class BloomFilter : public SingleFilterInterface
{
public:
	BloomFilter(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BloomFilter();

public:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) override;
};

