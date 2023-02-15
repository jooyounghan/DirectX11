#include <iostream>

#include "Shader.h"
#include "D3D11Utilizer.h"

// Test
#include "FileReader.h"


using namespace std;

D3D11Utilizer::D3D11Utilizer(int& buffer_width, int& buffer_height)
	: m_bufffer_width_(buffer_width), m_bufffer_height_(buffer_height), m_viewport_(D3D11_VIEWPORT{})
{
}

D3D11Utilizer::~D3D11Utilizer()
{
}

ComPtr<ID3D11Device> D3D11Utilizer::GetDevice()
{
	return m_device_;
}

ComPtr<ID3D11DeviceContext> D3D11Utilizer::GetDeviceContext()
{
	return m_device_context_;
}

bool D3D11Utilizer::IsSwappable()
{
	return m_swap_chain_ ? true : false;
}

bool D3D11Utilizer::InitDirectX11(HWND window_handler)
{
	if (!CreateDeviceAndSwapChain(window_handler))
	{
		return false;
	}

	if (!CreateStage())
	{
		return false;
	}

	if (!CreateRenderTargetView())
	{
		return false;
	}

	if (!CreateRasterizerState())
	{
		return false;
	}

	if (!CreateDepthBuffer())
	{
		return false;
	}

	if (!CreateDepthStencilState())
	{
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDeviceAndSwapChain(HWND window_handler)
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
	sd.BufferDesc.Width = m_bufffer_width_;
	sd.BufferDesc.Height = m_bufffer_height_;
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

	if (FAILED(temp_device.As(&m_device_))) {
		COUTERR("temp_device.As() Failed.");
		return false;
	}

	if (FAILED(temp_context.As(&m_device_context_))) {
		COUTERR("temp_context.As() Failed.");
		return false;
	}

	if (FAILED(temp_swap_chain.As(&m_swap_chain_))) {
		COUTERR("temp_swap_chain.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> back_buffer;
	ComPtr<ID3D11RenderTargetView>		temp_rt_view;
	ComPtr<ID3D11ShaderResourceView>	temp_sr_view;

	m_swap_chain_->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
	if (back_buffer)
	{
		if (FAILED(m_device_->CreateRenderTargetView(back_buffer.Get(), nullptr, temp_rt_view.GetAddressOf())))
		{
			COUTERR("CreateRenderTargetView Failed.");
			return false;
		}
		if (FAILED(m_device_->CreateShaderResourceView(back_buffer.Get(), nullptr, temp_sr_view.GetAddressOf())))
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

	if (FAILED(temp_rt_view.As(&m_rt_view_)))
	{
		COUTERR("temp_rt_view.As() Failed.");
		return false;
	}

	if (FAILED(temp_sr_view.As(&m_sr_view_)))
	{
		COUTERR("temp_sr_view.As() Failed.");
		return false;
	}
	return true;
}

bool D3D11Utilizer::CreateRasterizerState()
{
	ComPtr<ID3D11RasterizerState> temp_rasterizer_state;

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;

	if (FAILED(m_device_->CreateRasterizerState(&rd, temp_rasterizer_state.GetAddressOf())))
	{
		COUTERR("CreateRasterizerState Failed.");
		return false;
	}

	if (FAILED(temp_rasterizer_state.As(&m_rasterizer_state_)))
	{
		COUTERR("temp_rasterizer_state.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDepthBuffer()
{
	ComPtr<ID3D11Texture2D> depth_buffer;
	ComPtr<ID3D11DepthStencilView> temp_ds_view;

	D3D11_TEXTURE2D_DESC dd;
	ZeroMemory(&dd, sizeof(D3D11_TEXTURE2D_DESC));
	dd.Width = m_bufffer_width_;
	dd.Height = m_bufffer_height_;
	dd.MipLevels = 1;
	dd.ArraySize = 1;
	dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dd.SampleDesc.Count = 1;
	dd.SampleDesc.Quality = 0;
	dd.Usage = D3D11_USAGE_DEFAULT;
	dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dd.CPUAccessFlags = 0;
	dd.MiscFlags = 0;

	if (FAILED(m_device_->CreateTexture2D(&dd, 0, depth_buffer.GetAddressOf())))
	{
		COUTERR("CreateTexture2D For Depth Buffer Failed.");
		return false;
	}
	if (FAILED(m_device_->CreateDepthStencilView(depth_buffer.Get(), 0, temp_ds_view.GetAddressOf())))
	{
		COUTERR("CreateDepthStencilView Failed.");
		return false;
	}

	if (FAILED(temp_ds_view.As(&m_ds_view_)))
	{
		COUTERR("temp_ds_view.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateDepthStencilState()
{
	ComPtr<ID3D11DepthStencilState>	temp_ds_state_;
	D3D11_DEPTH_STENCIL_DESC dd;

	ZeroMemory(&dd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dd.DepthEnable = true; // false
	dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dd.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	if (FAILED(m_device_->CreateDepthStencilState(&dd, temp_ds_state_.GetAddressOf())))
	{
		cout << "CreateDepthStencilState() failed." << endl;
		return false;
	}

	if (FAILED(temp_ds_state_.As(&m_ds_state_)))
	{
		COUTERR("temp_ds_state_.As() Failed.");
		return false;
	}

	return true;
}

bool D3D11Utilizer::CreateStage()
{
	m_stage_ = make_shared<Stage>(m_device_, m_device_context_);
	return m_stage_.get() ? true : false;
}


void D3D11Utilizer::SetViewPort(const float& view_port_width, const float& view_port_height, const float& top_leftx, const float& top_lefty)
{
	// Set the viewport
	ZeroMemory(&m_viewport_, sizeof(D3D11_VIEWPORT));
	m_viewport_.TopLeftX = top_leftx;
	m_viewport_.TopLeftY = top_lefty;
	m_viewport_.Width = view_port_width;
	m_viewport_.Height = view_port_height;
	m_viewport_.MinDepth = 0.0f;
	m_viewport_.MaxDepth = 1.0f;

	m_device_context_->RSSetViewports(1, &m_viewport_);
}


void D3D11Utilizer::SwapChain()
{
	m_swap_chain_->Present(1, 0);
}

void D3D11Utilizer::OnResize(WPARAM w_param, LPARAM l_param)
{
	m_rt_view_->Release();
	m_rt_view_.Detach();

	m_sr_view_->Release();
	m_sr_view_.Detach();

	m_ds_view_->Release();
	m_ds_view_.Detach();

	m_swap_chain_->ResizeBuffers(0,
		(UINT)LOWORD(l_param),
		(UINT)HIWORD(l_param),
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	CreateRenderTargetView();
	CreateDepthBuffer();
}

void D3D11Utilizer::Render()
{
	SetViewPort((float)m_bufffer_width_, (float)m_bufffer_height_);

	float clear_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	m_device_context_->ClearRenderTargetView(m_rt_view_.Get(), clear_color);
	m_device_context_->ClearDepthStencilView(m_ds_view_.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	m_device_context_->OMSetRenderTargets(1, m_rt_view_.GetAddressOf(),
		m_ds_view_.Get());

	m_device_context_->OMSetDepthStencilState(m_ds_state_.Get(), 0);
	m_device_context_->RSSetState(m_rasterizer_state_.Get());

	m_stage_->Render();
}
