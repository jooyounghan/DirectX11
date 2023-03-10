#include <iostream>

#include "Shader.h"
#include "BaseMeshShader.h"

#include "D3D11Utilizer.h"

using namespace std;


bool D3D11Utilizer::InitDirectX11(
	HWND window_handler,
	const int& buffer_width,
	const int& buffer_height,
	OUT ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11DeviceContext>& context,
	OUT ComPtr<IDXGISwapChain>& swap_chain,
	OUT ComPtr<ID3D11RenderTargetView>& rt_view,
	OUT ComPtr<ID3D11ShaderResourceView>& sr_view,
	OUT ComPtr<ID3D11RasterizerState>& rs_state,
	OUT ComPtr<ID3D11DepthStencilView>& ds_view,
	OUT ComPtr<ID3D11DepthStencilState>	ds_state
)
{
	if (!CreateDeviceAndSwapChain(window_handler, buffer_width, buffer_height, device, context, swap_chain))
	{
		return false;
	}

	if (!CreateRenderTargetView(device, swap_chain, rt_view, sr_view))
	{
		return false;
	}

	if (!CreateRasterizerState(device, rs_state))
	{
		return false;
	}

	if (!CreateDepthBuffer(buffer_width, buffer_height, device, ds_view))
	{
		return false;
	}

	if (!CreateDepthStencilState(device, ds_state))
	{
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDeviceAndSwapChain(HWND window_handler,
	const int& buffer_width,
	const int& buffer_height,
	OUT ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11DeviceContext>& context,
	OUT ComPtr<IDXGISwapChain>& swap_chain)
{
	ComPtr<ID3D11Device> temp_device;
	ComPtr<ID3D11DeviceContext> temp_context;
	ComPtr<IDXGISwapChain> temp_swap_chain;

	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	const D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	const UINT numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = buffer_width;
	sd.BufferDesc.Height = buffer_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window_handler;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;



	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		temp_swap_chain.GetAddressOf(),
		temp_device.GetAddressOf(),
		&FeatureLevelsSupported,
		temp_context.GetAddressOf()
	)))
	{
		COUTERR("D3D11CreateDeviceAndSwapChain Failed.");
		return false;
	}

	if (FAILED(temp_device.As(&device))) {
		COUTERR("temp_device.As() Failed.");
		return false;
	}

	if (FAILED(temp_context.As(&context))) {
		COUTERR("temp_context.As() Failed.");
		return false;
	}

	if (FAILED(temp_swap_chain.As(&swap_chain))) {
		COUTERR("temp_swap_chain.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateRenderTargetView(
	ComPtr<ID3D11Device>& device,
	ComPtr<IDXGISwapChain>& swap_chain,
	OUT ComPtr<ID3D11RenderTargetView>& rt_view_,
	OUT ComPtr<ID3D11ShaderResourceView>& sr_view_
)
{
	ComPtr<ID3D11Texture2D> back_buffer;
	ComPtr<ID3D11RenderTargetView>		temp_rt_view;
	ComPtr<ID3D11ShaderResourceView>	temp_sr_view;

	swap_chain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
	if (back_buffer)
	{
		if (FAILED(device->CreateRenderTargetView(back_buffer.Get(), nullptr, temp_rt_view.GetAddressOf())))
		{
			COUTERR("CreateRenderTargetView Failed.");
			return false;
		}
		if (FAILED(device->CreateShaderResourceView(back_buffer.Get(), nullptr, temp_sr_view.GetAddressOf())))
		{
			COUTERR("CreateShaderResourceView Failed.");
			return false;
		}		
	}
	else
	{
		COUTERR("Get Back Buffer From Swap Chain Failed");
		return false;
	}

	if (FAILED(temp_rt_view.As(&rt_view_)))
	{
		COUTERR("temp_rt_view.As() Failed.");
		return false;
	}

	if (FAILED(temp_sr_view.As(&sr_view_)))
	{
		COUTERR("temp_sr_view.As() Failed.");
		return false;
	}
	return true;
}

bool D3D11Utilizer::CreateRasterizerState(
	ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11RasterizerState>& rs_state
	)
{
	ComPtr<ID3D11RasterizerState> temp_rasterizer_state;

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = false;
	//rd.DepthClipEnable = true;

	if (FAILED(device->CreateRasterizerState(&rd, temp_rasterizer_state.GetAddressOf())))
	{
		COUTERR("CreateRasterizerState Failed.");
		return false;
	}

	if (FAILED(temp_rasterizer_state.As(&rs_state)))
	{
		COUTERR("temp_rasterizer_state.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDepthBuffer(
	const int& buffer_width,
	const int& buffer_height,
	ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11DepthStencilView>& ds_view
)
{
	ComPtr<ID3D11Texture2D> depth_buffer;
	ComPtr<ID3D11DepthStencilView> temp_ds_view;

	D3D11_TEXTURE2D_DESC dd;
	ZeroMemory(&dd, sizeof(D3D11_TEXTURE2D_DESC));
	dd.Width = buffer_width;
	dd.Height = buffer_height;
	dd.MipLevels = 1;
	dd.ArraySize = 1;
	dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dd.SampleDesc.Count = 1;
	dd.SampleDesc.Quality = 0;
	dd.Usage = D3D11_USAGE_DEFAULT;
	dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dd.CPUAccessFlags = 0;
	dd.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&dd, 0, depth_buffer.GetAddressOf())))
	{
		COUTERR("CreateTexture2D For Depth Buffer Failed.");
		return false;
	}
	if (FAILED(device->CreateDepthStencilView(depth_buffer.Get(), 0, temp_ds_view.GetAddressOf())))
	{
		COUTERR("CreateDepthStencilView Failed.");
		return false;
	}

	if (FAILED(temp_ds_view.As(&ds_view)))
	{
		COUTERR("temp_ds_view.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDepthStencilState(
	ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11DepthStencilState>	ds_state
)
{
	ComPtr<ID3D11DepthStencilState>	temp_ds_state_;
	D3D11_DEPTH_STENCIL_DESC dd;

	ZeroMemory(&dd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dd.DepthEnable = true; // false
	dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dd.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	if (FAILED(device->CreateDepthStencilState(&dd, temp_ds_state_.GetAddressOf())))
	{
		cout << "CreateDepthStencilState() failed." << endl;
		return false;
	}

	if (FAILED(temp_ds_state_.As(&ds_state)))
	{
		COUTERR("temp_ds_state_.As() Failed.");
		return false;
	}

	return true;
}


void D3D11Utilizer::SetViewPort(ComPtr<ID3D11DeviceContext>& context,
	const float& view_port_width,
	const float& view_port_height,
	const float& top_leftx,
	const float& top_lefty,
	OUT D3D11_VIEWPORT& view_port)
{
	// Set the viewport
	ZeroMemory(&view_port, sizeof(D3D11_VIEWPORT));
	view_port.TopLeftX = top_leftx;
	view_port.TopLeftY = top_lefty;
	view_port.Width = view_port_width;
	view_port.Height = view_port_height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	context->RSSetViewports(1, &view_port);
}

void D3D11Utilizer::OnResize(
	const int& width,
	const int& height,
	ComPtr<ID3D11Device>& device,
	OUT ComPtr<ID3D11RenderTargetView>& rt_view,
	OUT ComPtr<ID3D11ShaderResourceView>& sr_view,
	OUT ComPtr<ID3D11DepthStencilView>& ds_view,
	OUT ComPtr<IDXGISwapChain>& swap_chain
)
{
	rt_view->Release();
	rt_view.Detach();

	sr_view->Release();
	sr_view.Detach();

	ds_view->Release();
	ds_view.Detach();

	swap_chain->ResizeBuffers(0,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	CreateRenderTargetView(device, swap_chain, rt_view, sr_view);
	CreateDepthBuffer(width, height, device, ds_view);
}

void D3D11Utilizer::Orthonormalize(Vector3& v1, Vector3& v2, Vector3& v3)
{
	// Normalize the first vector.
	v1.Normalize();

	// Project the second vector onto the first vector and subtract it to make it orthogonal.
	v2 -= v1 * v2.Dot(v1);

	// Normalize the second vector.
	v2.Normalize();

	// Project the third vector onto the first and second vectors and subtract it to make it orthogonal.
	v3 -= (v3 * v3.Dot(v1) + v2 * v3.Dot(v2));

	// Normalize the third vector.
	v3.Normalize();
}


void D3D11Utilizer::GramSchmidt(IN OUT Matrix& matrix)
{
		// Extract the three rows of the matrix that correspond to the x, y, and z axes.
		Vector3 xRow(matrix._11, matrix._12, matrix._13);
		Vector3 yRow(matrix._21, matrix._22, matrix._23);
		Vector3 zRow(matrix._31, matrix._32, matrix._33);

		// Orthonormalize the rows using the Gram-Schmidt process.
		Orthonormalize(xRow, yRow, zRow);

		// Store the orthonormalized rows back into the matrix.
		matrix._11 = xRow.x; matrix._12 = xRow.y; matrix._13 = xRow.z;
		matrix._21 = yRow.x; matrix._22 = yRow.y; matrix._23 = yRow.z;
		matrix._31 = zRow.x; matrix._32 = zRow.y; matrix._33 = zRow.z;

}

