#include "BloomDown.h"
#include "ID3D11Helper.h"

BloomDownShader::BloomDownShader(ComPtr<ID3D11Device>& cpDeviceIn)
	: cpDevice(cpDeviceIn)
{
	ID3D11Helper::CreateVSInputLayOut(
		cpDevice.Get(),
		L"BloomDownVS.hlsl",
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		},
		cpBloomDownVS.GetAddressOf(),
		cpBloomDownInputLayout.GetAddressOf()
	);

	ID3D11Helper::CreatePS(
		cpDevice.Get(),
		L"BloomDownPS.hlsl",
		cpBloomDownPS.GetAddressOf()
	);
}

BloomDownShader& BloomDownShader::GetIncetance(ComPtr<ID3D11Device>& cpDeviceIn)
{
	static BloomDownShader s(cpDeviceIn);
	return s;
}

BloomDown::BloomDown(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& cpInputTexture2DIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: FilterInterface(cpDeviceIn, cpDeviceContextIn, cpInputTexture2DIn, uiWidthIn, uiHeightIn)
{

}

BloomDown::~BloomDown()
{
}

void BloomDown::CreateOutputResource()
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth / 2, uiHeight / 2, 1, 0, D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpOutputTexture2D.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputRTV.GetAddressOf());
}

void BloomDown::StartFilter()
{
}
