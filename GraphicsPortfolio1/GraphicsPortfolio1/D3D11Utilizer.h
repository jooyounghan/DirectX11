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
	static bool InitDirectX11(
		HWND window_handler,
		const UINT& buffer_width,
		const UINT& buffer_height,
		OUT ComPtr<ID3D11Device>& device,
		OUT ComPtr<ID3D11DeviceContext>& context,
		OUT ComPtr<IDXGISwapChain>& swap_chain,
		OUT ComPtr<ID3D11RenderTargetView>& rt_view,
		OUT ComPtr<ID3D11ShaderResourceView>& sr_view,
		OUT ComPtr<ID3D11RasterizerState>& rs_state,
		OUT ComPtr<ID3D11DepthStencilView>&	ds_view,
		OUT ComPtr<ID3D11DepthStencilState>	ds_state
	);

public:
	static bool CreateDeviceAndSwapChain(
		HWND window_handler,
		const UINT& buffer_width,
		const UINT& buffer_height,
		OUT ComPtr<ID3D11Device>& device,
		OUT ComPtr<ID3D11DeviceContext>& context,
		OUT ComPtr<IDXGISwapChain>& swap_chain
	);

	static bool CreateRenderTargetView(
		ComPtr<ID3D11Device>& device,
		ComPtr<IDXGISwapChain>& swap_chain,
		OUT ComPtr<ID3D11RenderTargetView>&	rt_view,
		OUT ComPtr<ID3D11ShaderResourceView>& sr_view
	);

	static bool CreateRasterizerState
	(
		ComPtr<ID3D11Device>& device,
		OUT ComPtr<ID3D11RasterizerState>& rs_state
	);
	
	static bool CreateDepthBuffer(
		const UINT& buffer_width,
		const UINT& buffer_height,
		ComPtr<ID3D11Device>& device,
		OUT ComPtr<ID3D11DepthStencilView>& ds_view
	);

	static bool CreateDepthStencilState(
		ComPtr<ID3D11Device>& device, 
		OUT ComPtr<ID3D11DepthStencilState>	ds_state
	);

	static void SetViewPort(
		ComPtr<ID3D11DeviceContext>& context,
		const float& view_port_width,
		const float& view_port_height,
		const float& top_leftx,
		const float& top_lefty,
		OUT D3D11_VIEWPORT& view_port
	);

	static void OnResize(
		const UINT& width,
		const UINT& height,
		ComPtr<ID3D11Device>& device,
		OUT ComPtr<ID3D11RenderTargetView>& rt_view,
		OUT ComPtr<ID3D11ShaderResourceView>& sr_view,
		OUT ComPtr<ID3D11DepthStencilView>& ds_view,
		OUT ComPtr<IDXGISwapChain>& swap_chain
	);

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
		index_count = (unsigned int)index_data.size();

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
};

