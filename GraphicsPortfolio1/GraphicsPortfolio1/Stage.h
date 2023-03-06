#pragma once

#include "MeshGroup.h"
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
	vector<std::shared_ptr<MeshGroup>>	m_mesh_group_;

public:
	std::shared_ptr<Camera>	m_main_camera_;
	std::shared_ptr<Light>	m_lights_;

public:
	vector<std::weak_ptr<IRenderable>> m_renderable_group_;

public:
	void AddModel(const ModelData& model_data);
	void RemoveModel(const size_t& index);
	void SetModelTransformed(const size_t& index, const ModelData& model_data);

public:
	void Update();
	void Render();
};

