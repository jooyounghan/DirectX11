#pragma once

#include "IMeshGroup.h"
#include "Light.h"
#include "Camera.h"

class Stage
{
public:
	Stage(int& buffer_width, int& buffer_height);
	~Stage();
	
public:
	bool InitStage(HWND window_handle);

public:
	ComPtr<ID3D11Device>				m_device_;
	ComPtr<ID3D11DeviceContext>			m_device_context_;
	ComPtr<IDXGISwapChain>				m_swap_chain_;
	ComPtr<ID3D11RenderTargetView>		m_rt_view_;
	ComPtr<ID3D11ShaderResourceView>	m_sr_view_;

	ComPtr<ID3D11RasterizerState>		m_rasterizer_state_;
	ComPtr<ID3D11DepthStencilView>		m_ds_view_;
	ComPtr<ID3D11DepthStencilState>		m_ds_state_;
	D3D11_VIEWPORT						m_viewport_;

public:
	ComPtr<ID3D11Device>				GetDevice();
	ComPtr<ID3D11DeviceContext>			GetDeviceContext();

public:
	bool IsSwappable();
	void OnResize();

protected:
	int& m_buffer_width_;
	int& m_buffer_height_;

public:
	vector<shared_ptr<IMeshGroup>>	m_mesh_group_;
	vector<LightConstantData>		m_lights_group_;
	
public:
	shared_ptr<Camera> m_main_camera_;

public:
	void AddModel(const string& file_path, const string& file_name);
	void RemoveModel(const size_t& index);

public:
	void Update();
	void Render();
};

