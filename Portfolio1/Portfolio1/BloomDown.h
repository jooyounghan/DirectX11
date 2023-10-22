#pragma once
#include "FilterInterface.h"

class BloomDownShader
{
private:
	BloomDownShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	BloomDownShader(const BloomDownShader& ref){}
	BloomDownShader& operator=(const BloomDownShader& ref) {}
	~BloomDownShader() {}

public:
	static BloomDownShader& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBloomDownVS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBloomDownInputLayout;

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBloomDownPS;
};


class BloomDown : public FilterInterface
{
public:
	BloomDown(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BloomDown();

public:
	virtual void CreateOutputResource(DXGI_FORMAT eFormat) override;
	virtual void StartFilter(ID3D11ShaderResourceView** ppInputSRV) override;
};

