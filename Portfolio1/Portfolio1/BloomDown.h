#pragma once
#include "FilterInterface.h"

class BloomDownShader
{
private:
	BloomDownShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	BloomDownShader(const BloomDownShader& ref) : cpDevice(ref.cpDevice) {}
	BloomDownShader& operator=(const BloomDownShader& ref) {}
	~BloomDownShader() {}

public:
	static BloomDownShader& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;

public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBloomDownVS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBloomDownInputLayout;

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBloomDownPS;
};

class BloomDown : public FilterInterface
{
	BloomDown(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpInputTexture2DIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);

	virtual ~BloomDown();

public:
	virtual void CreateOutputResource() override;
	virtual void StartFilter() override;
};

