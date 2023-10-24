#pragma once
#include "SingleFilterInterface.h"

class BloomShader
{
private:
	BloomShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	BloomShader(const BloomShader& ref) {}
	BloomShader& operator=(const BloomShader& ref) {}
	~BloomShader() {}

public:
	static BloomShader& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBloomPS;
};

class BloomFilter : public SingleFilterInterface
{
public:
	BloomFilter(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BloomFilter();

public:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) override;
};

