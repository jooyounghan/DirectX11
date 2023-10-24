#pragma once
#include "BlendFilterInterface.h"

class BlendShader
{
private:
	BlendShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);
	BlendShader(const BlendShader& ref) {}
	BlendShader& operator=(const BlendShader& ref) {}
	~BlendShader() {}

public:
	static BlendShader& GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn);

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBlendPS;
};

class BlendFilter : public BlendFilterInterface
{
public:
	BlendFilter(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormat
	);

	virtual ~BlendFilter();

public:
	virtual void StartFilter(ID3D11ShaderResourceView** ppInput1SRV, ID3D11ShaderResourceView** ppInput2SRV) override;
};
