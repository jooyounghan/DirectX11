#pragma once
#include "FilterInterface.h"

class BloomUpShader
{
private:
	BloomUpShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	BloomUpShader(const BloomUpShader& ref) : cpDevice(ref.cpDevice) {}
	BloomUpShader& operator=(const BloomUpShader& ref) {}
	~BloomUpShader() {}

public:
	static BloomUpShader& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;

public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBloomUpVS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBloomUpInputLayout;

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBloomUpPS;
};

class BloomUp : public FilterInterface
{
public:
	BloomUp(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BloomUp();

public:
	virtual void CreateOutputResource(DXGI_FORMAT eFormat) override;
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) override;
};

