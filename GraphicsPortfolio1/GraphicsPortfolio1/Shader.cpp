#include "Shader.h"
#include <iostream>

using namespace std;

Shader::Shader(ComPtr<ID3D11Device>& device, const wstring& vs_name, const wstring& ps_name)
	: m_device_(device)
{
	UINT compile_flags = 0;
	#if defined(DEBUG) || defined(_DEBUG)
	compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	// TODO : GetAddressOf()를 &로 변경

	if (FAILED(D3DCompileFromFile(vs_name.c_str(), NULL, NULL, "main", "vs_5_0", compile_flags, 0, m_vs_blob_.GetAddressOf(), NULL)))
	{
		wcout << "D3DCompileFromFile For Vertex Shader Failed. File Name : " << vs_name << endl;
	}

	if (FAILED(D3DCompileFromFile(ps_name.c_str(), NULL, NULL, "main", "ps_5_0", compile_flags, 0, m_ps_blob_.GetAddressOf(), NULL)))
	{
		wcout << "D3DCompileFromFile For Pixel Shader Failed. File Name : " << vs_name << endl;
	}

	if (FAILED(m_device_->CreateVertexShader(m_vs_blob_->GetBufferPointer(), m_vs_blob_->GetBufferSize(), NULL, m_vertex_shader_.GetAddressOf())))
	{
		wcout << "CreateVertexShader From Vertex Shader Blob Failed" << vs_name << endl;
	}

	if (FAILED(m_device_->CreatePixelShader(m_ps_blob_->GetBufferPointer(), m_ps_blob_->GetBufferSize(), NULL, m_pixel_shader_.GetAddressOf())))
	{
		wcout << "CreateVertexShader From Vertex Shader Blob Failed" << vs_name << endl;
	}
}

Shader::~Shader()
{

	m_vs_blob_->Release();
	m_vs_blob_.Detach();

	m_ps_blob_->Release();
	m_ps_blob_.Detach();

	m_vertex_shader_->Release();
	m_vertex_shader_.Detach();

	m_pixel_shader_->Release();
	m_pixel_shader_.Detach();
}


