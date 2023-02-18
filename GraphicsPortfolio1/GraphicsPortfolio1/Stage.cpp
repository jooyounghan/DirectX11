#include <directxtk/SimpleMath.h>

#include "Stage.h"

using namespace DirectX;

Stage::Stage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context, int& buffer_width, int& buffer_height)
	: m_device_(device), m_device_context_(device_context), m_bufffer_width_(buffer_width), m_bufffer_height_(buffer_height)
{
	// 추후 Camera 객체를 만들어서 처리
	m_stage_vertex_constant_.view = Matrix::CreateTranslation(0.0f, 0.0f, 50.0f).Transpose();

	const float aspect = (float)m_bufffer_width_ / (float)m_bufffer_height_;
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;

	Matrix projRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), aspect, m_nearZ, m_farZ);
	m_stage_vertex_constant_.projection = projRow.Transpose();

}

Stage::~Stage()
{

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


void Stage::AddMeshGroup(shared_ptr<IMeshGroup> mesh_group)
{

	m_mesh_group_.push_back(mesh_group);
}

void Stage::RemoveMeshGroup(const size_t& index)
{
	m_mesh_group_.erase(m_mesh_group_.begin() + index);
}

void Stage::Render()
{
	//SetViewPort((float)m_bufffer_width_, (float)m_bufffer_height_);

	//float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//m_device_context_->ClearRenderTargetView(m_rt_view_.Get(), clear_color);
	//m_device_context_->ClearDepthStencilView(m_ds_view_.Get(),
	//	D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	//	1.0f, 0);

	//m_device_context_->OMSetRenderTargets(1, m_rt_view_.GetAddressOf(),
	//	m_ds_view_.Get());

	//m_device_context_->OMSetDepthStencilState(m_ds_state_.Get(), 0);
	//m_device_context_->RSSetState(m_rasterizer_state_.Get());


	//for (auto& mesh : m_mesh_group_)
	//{
	//	mesh->Render(m_device_context_);
	//}
}

//void D3D11Utilizer::AddModel(const string& file_path, const string& file_name)
//{
//
//	vector<MeshData> mesh_data = FileReader::GetMeshDataFromFile(file_path, file_name);
//	shared_ptr<IMeshGroup> mesh_group = make_shared<IMeshGroup>(m_device_, mesh_data);
//
//	// Test ==============================================================
//
//
//	// Test ==============================================================
//
//	mesh_group->m_mesh_shader_ = make_shared<BaseMeshShader>(m_device_, L"BaseVertexShader.hlsl", L"BasePixelShader.hlsl");
//	m_stage_->AddMeshGroup(mesh_group);
//}
//
//void D3D11Utilizer::RemoveModel(const size_t& index)
//{
//	m_stage_->RemoveMeshGroup(index);
//}
//
