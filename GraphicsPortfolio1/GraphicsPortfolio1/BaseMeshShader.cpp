#include "BaseMeshShader.h"

#include <iostream>

using namespace std;

BaseMeshShader::BaseMeshShader(ComPtr<ID3D11Device>& device, const wstring& vs_name, const wstring& ps_name)
	: Shader(device, vs_name, ps_name)
{
	vector<D3D11_INPUT_ELEMENT_DESC> input_descs = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 2 * 3 * 4 , D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(m_device_->CreateInputLayout(input_descs.data(), UINT(input_descs.size()),
		m_vs_blob_->GetBufferPointer(),
		m_vs_blob_->GetBufferSize(), m_input_layout_.GetAddressOf())))
	{
		cout << "CreateInputLayout Failed" << endl;
	}

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(m_device_->CreateSamplerState(&sd, m_sampler_state_.GetAddressOf())))
	{
		cout << "CreateSamplerState Failed" << endl;
	}
}

BaseMeshShader::~BaseMeshShader()
{
}
