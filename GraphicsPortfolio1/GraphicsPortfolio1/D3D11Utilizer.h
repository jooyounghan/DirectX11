#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>

#include <iostream>
#include <vector>

#include "FileReader.h"
#include "Stage.h"

#include "Delegator.h"

#define COUTERR(String) std::cout << String << "/" << "Error Code : " << GetLastError() << std::endl

using Microsoft::WRL::ComPtr;
using std::vector;
using std::cout;
using std::endl;

class D3D11Utilizer
{
public:
	D3D11Utilizer(int& buffer_width, int& buffer_height);
	~D3D11Utilizer();

protected:
	int& m_bufffer_width_;
	int& m_bufffer_height_;

public:
	// App으로 이동하는거 생각하기
	ComPtr<ID3D11Device>				m_device_;
	ComPtr<ID3D11DeviceContext>			m_device_context_;
	ComPtr<ID3D11RenderTargetView>		m_rt_view_;
	ComPtr<ID3D11ShaderResourceView>	m_sr_view_;
	ComPtr<IDXGISwapChain>				m_swap_chain_;
	ComPtr<ID3D11RasterizerState>		m_rasterizer_state_;
	ComPtr<ID3D11DepthStencilView>		m_ds_view_;
	ComPtr<ID3D11DepthStencilState>		m_ds_state_;
	D3D11_VIEWPORT						m_viewport_;

public:
	ComPtr<ID3D11Device>				GetDevice();
	ComPtr<ID3D11DeviceContext>			GetDeviceContext();

public:
	bool IsSwappable();

public:
	bool InitDirectX11(HWND window_handler);

public:
	bool CreateDeviceAndSwapChain(HWND window_handler);
	bool CreateRenderTargetView();
	bool CreateRasterizerState();
	bool CreateDepthBuffer();
	bool CreateDepthStencilState();

public:
	bool CreateStage();

public:
	template<typename VERTEX>
	static void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const vector<VERTEX>& vertex_data, ComPtr<ID3D11Buffer>& vertex_buffer)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = UINT(sizeof(VERTEX) * vertex_data.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.StructureByteStride = sizeof(VERTEX);

		D3D11_SUBRESOURCE_DATA ssd;
		ssd.pSysMem = vertex_data.data();
		ssd.SysMemPitch = 0;
		ssd.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(&bd, &ssd, vertex_buffer.GetAddressOf())))
		{
			cout << "Creating Vertex Buffer Failed" << endl;
		}
	}

	template<typename INDICE>
	static void CreateIndexBuffer(const ComPtr<ID3D11Device>& device, const vector<INDICE>& index_data, ComPtr<ID3D11Buffer>& index_buffer, OUT unsigned int& index_count)
	{
		index_count = index_data.size();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = UINT(sizeof(INDICE) * index_data.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.StructureByteStride = sizeof(INDICE);

		D3D11_SUBRESOURCE_DATA ssd;
		ssd.pSysMem = index_data.data();
		ssd.SysMemPitch = 0;
		ssd.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(&bd, &ssd, index_buffer.GetAddressOf())))
		{
			cout << "Creating Index Buffer Failed" << endl;
		}
	}

	template<typename CONSTANT>
	static void CreateConstantBuffer(const ComPtr<ID3D11Device>& device, const CONSTANT& constant_data, ComPtr<ID3D11Buffer>& constant_buffer)
	{
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = UINT(sizeof(constant_data));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA ssd;
		ssd.pSysMem = &constant_data;
		ssd.SysMemPitch = 0;
		ssd.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(&bd, &ssd, constant_buffer.GetAddressOf())))
		{
			cout << "Creating Constant Buffer Failed" << endl;
		}
	}

	static void CreateTexture(const ComPtr<ID3D11Device>& device, const Image& texture_data, ComPtr<ID3D11Texture2D>& texture_buffer, ComPtr<ID3D11ShaderResourceView>& ts_view)
	{
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = texture_data.width;
		td.Height = texture_data.height;
		td.MipLevels = td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_IMMUTABLE;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA ssd;
		ssd.pSysMem = texture_data.image_data_.data();
		ssd.SysMemPitch = texture_data.width * sizeof(uint8_t) * 4;

		if (FAILED(device->CreateTexture2D(&td, &ssd, texture_buffer.GetAddressOf())))
		{
			cout << "CreateTexture2D Failed" << endl;
			return;
		}

		if (FAILED(device->CreateShaderResourceView(texture_buffer.Get(), nullptr,
			ts_view.GetAddressOf())))
		{
			cout << "CreateShaderResourceView Failed" << endl;
			return;
		}
	}

	template<typename DYNAMIC_UPDATE>
	static void UpdateBuffer(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext> context,
		const DYNAMIC_UPDATE& update_data, ComPtr<ID3D11Buffer>& update_buffer)
	{
		if (!update_buffer)
		{
			cout << "UpdateBuffer() buffer was not initialized." << endl;
			return;
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		context->Map(update_buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &update_data, sizeof(update_data));
		context->Unmap(update_buffer.Get(), NULL);
	}

public:
	void SetViewPort(const float& view_port_width, const float& view_port_height, const float& top_leftx = 0.f, const float& top_lefty = 0.f);

public:
	void SwapChain();

public:
	void OnResize(WPARAM w_param, LPARAM l_param);

public:
	shared_ptr<Stage>	m_stage_;

public:
	void Render();

public:
	void AddModel(const string& path);
};

