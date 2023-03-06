#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl.h>
#include <iostream>
#include <vector>
#include <string>

using Microsoft::WRL::ComPtr;

class Shader
{
public:
	Shader(ComPtr<ID3D11Device>& device, const std::wstring& vs_name, const std::wstring& ps_name);
	~Shader();

protected:
	ComPtr<ID3D11Device>& m_device_;

public:
	ComPtr<ID3D11InputLayout>	m_input_layout_;
	ComPtr<ID3D11SamplerState>	m_sampler_state_;

	ComPtr<ID3D11VertexShader>	m_vertex_shader_;
	ComPtr<ID3D11PixelShader>	m_pixel_shader_;

	ComPtr<ID3D10Blob> m_vs_blob_;
	ComPtr<ID3D10Blob> m_ps_blob_;

};

