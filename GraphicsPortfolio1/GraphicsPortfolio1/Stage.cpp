#include "Stage.h"
#include "FileReader.h"
#include "BaseMeshShader.h"
#include "D3D11Utilizer.h"

using namespace DirectX;
using namespace std;

Stage::Stage(int& buffer_width, int& buffer_height)
	: m_buffer_width_(buffer_width), m_buffer_height_(buffer_height), m_viewport_()
{

}

Stage::~Stage()
{

}

bool Stage::InitStage(HWND window_handle)
{
	bool result = D3D11Utilizer::InitDirectX11(
		window_handle,
		m_buffer_width_,
		m_buffer_height_,
		m_device_,
		m_device_context_,
		m_swap_chain_,
		m_rt_view_,
		m_sr_view_,
		m_rasterizer_state_,
		m_ds_view_,
		m_ds_state_
	);

	if (!result)
	{
		COUTERR("Initializing DirectX11 For Stage Failed.");
	}

	m_main_camera_ = make_shared<Camera>(m_device_, m_buffer_width_, m_buffer_height_);
	m_lights_ = make_shared<Light>(m_device_);
	return result;
}

ComPtr<ID3D11Device> Stage::GetDevice()
{
	return m_device_;
}

ComPtr<ID3D11DeviceContext> Stage::GetDeviceContext()
{
	return m_device_context_;
}

bool Stage::IsSwappable()
{
	return m_swap_chain_ ? true : false;
}

void Stage::OnResize()
{
	D3D11Utilizer::OnResize(m_buffer_width_, m_buffer_height_, m_device_, m_rt_view_, m_sr_view_, m_ds_view_, m_swap_chain_);
	D3D11Utilizer::SetViewPort(m_device_context_, (float)m_buffer_width_, (float)m_buffer_width_, 0.f, 0.f, m_viewport_);
}


void Stage::AddModel(const ModelData& model_data)
{
	shared_ptr<IMeshGroup> mesh_group = make_shared<IMeshGroup>(m_device_, model_data.mesh_data);
	mesh_group->m_mesh_shader_ = make_shared<BaseMeshShader>(m_device_, L"BaseVertexShader.hlsl", L"BasePixelShader.hlsl");
	m_mesh_group_.emplace_back(mesh_group);
}

void Stage::RemoveModel(const size_t& index)
{
	m_mesh_group_.erase(m_mesh_group_.begin() + index);
}

void Stage::SetModelTransformed(const size_t& index, const ModelData& model_data)
{	
	const Vector3 translation = Vector3(model_data.model_translation[0], model_data.model_translation[1], model_data.model_translation[2]);
	const Vector3 scaling = Vector3(model_data.model_scaling[0], model_data.model_scaling[1], model_data.model_scaling[2]);

	Matrix transformation = Matrix::CreateScale(scaling) *
		Matrix::CreateRotationX(model_data.model_rotation[0]) *
		Matrix::CreateRotationY(model_data.model_rotation[1]) *
		Matrix::CreateRotationZ(model_data.model_rotation[2]) *
		Matrix::CreateTranslation(translation);

	m_mesh_group_[index]->SetVertexConstantData(transformation);
	m_mesh_group_[index]->Update(m_device_, m_device_context_);
}


void Stage::Update()
{
	m_main_camera_->UpdateCamera(m_device_, m_device_context_);
	m_lights_->UpdateLight(m_device_, m_device_context_);
	for (auto& meshgroup : m_mesh_group_)
	{
		meshgroup->Update(m_device_, m_device_context_);
	}
}

void Stage::Render()
{
	D3D11Utilizer::SetViewPort(m_device_context_, (float)m_buffer_width_, (float)m_buffer_height_, 0.f, 0.f, m_viewport_);

	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	m_device_context_->ClearRenderTargetView(m_rt_view_.Get(), clear_color);
	m_device_context_->ClearDepthStencilView(m_ds_view_.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	m_device_context_->OMSetRenderTargets(1, m_rt_view_.GetAddressOf(),
		m_ds_view_.Get());

	m_device_context_->OMSetDepthStencilState(m_ds_state_.Get(), 0);
	m_device_context_->RSSetState(m_rasterizer_state_.Get());


	for (auto& mesh : m_mesh_group_)
	{
		mesh->Render(m_device_context_, m_main_camera_->m_vertex_camera_cbuffer_, m_lights_->m_light_cbuffer);
	}

}
