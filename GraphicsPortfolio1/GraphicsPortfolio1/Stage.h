#pragma once
#include <memory>
#include <wrl.h>


#include "IMeshGroup.h"
#include "Light.h"

using std::shared_ptr;
using Microsoft::WRL::ComPtr;

class Stage
{
public:
	Stage(int& buffer_width, int& buffer_height);
	~Stage();
	
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



protected:
	int& m_bufffer_width_;
	int& m_bufffer_height_;

public:
	ComPtr<ID3D11Device>& m_device_;
	ComPtr<ID3D11DeviceContext>& m_device_context_;

public:
	vector<shared_ptr<IMeshGroup>>	m_mesh_group_;
	vector<LightConstantData>		m_lights_group_;

public:
	MeshGroupVertexConstantData m_stage_vertex_constant_;

public:
	ComPtr<ID3D11Buffer> m_vertex_stage_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_stage_cbuffer_;


public:
	void AddModel(const string& file_path, const string& file_name);
	void RemoveModel(const size_t& index);

public:
	void AddMeshGroup(shared_ptr<IMeshGroup> mesh_group);
	void RemoveMeshGroup(const size_t& index);

public:
	void Render();
};

