#include "MeshGroup.h"
#include "BaseMeshShader.h"
#include "D3D11Utilizer.h"

using namespace std;

MeshGroup::MeshGroup(ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext>& device_context,
	shared_ptr<IRenderable> camera,
	shared_ptr<IRenderable> light)
	: IRenderable(device, device_context), m_camera_(camera), m_light_(light)
{
	InitConstantData(device);
	m_render_shader_ = make_shared<BaseMeshShader>(device);
}

MeshGroup::~MeshGroup()
{
}

void MeshGroup::AddMeshData(const vector<MeshData>& mesh_data_vector)
{
	for (auto& mesh_data : mesh_data_vector)
	{
		shared_ptr<Mesh> mesh = std::make_shared<Mesh>(m_device_, mesh_data);
		m_meshes_.push_back(mesh);
	}
}

void MeshGroup::InitConstantData(ComPtr<ID3D11Device>& device)
{
	m_vertex_constant_data_.model = Matrix::Identity;
	m_vertex_constant_data_.inv_tranpose = Matrix::Identity;
	D3D11Utilizer::CreateConstantBuffer<MeshVertexConstantData>(device, m_vertex_constant_data_, m_vertex_cbuffer_);

	m_pixel_constant_data_.diffuse = 0.f;
	m_pixel_constant_data_.specular = 0.f;
	m_pixel_constant_data_.shininess = 0.f;
	D3D11Utilizer::CreateConstantBuffer<MeshPixelConstantData>(device, m_pixel_constant_data_, m_pixel_cbuffer_);
}

void MeshGroup::SetVertexConstantData(const Matrix& model_tranform)
{
	m_vertex_constant_data_.model = model_tranform;
	m_vertex_constant_data_.inv_tranpose = m_vertex_constant_data_.model.Invert().Transpose();
	m_vertex_constant_data_.model = m_vertex_constant_data_.model.Transpose();
	m_vertex_constant_data_.inv_tranpose = m_vertex_constant_data_.inv_tranpose.Transpose();
}

void MeshGroup::SetPixelConstantData(const float& diffuse, const float& specular, const float& shininess)
{
	m_pixel_constant_data_.diffuse = diffuse;
	m_pixel_constant_data_.specular = specular;
	m_pixel_constant_data_.shininess = shininess;
}

void MeshGroup::Render()
{
	m_device_context_->IASetInputLayout(m_render_shader_->m_input_layout_.Get());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_device_context_->VSSetShader(m_render_shader_->m_vertex_shader_.Get(), 0, 0);
	m_device_context_->PSSetShader(m_render_shader_->m_pixel_shader_.Get(), 0, 0);
	m_device_context_->PSSetSamplers(0, 1, m_render_shader_->m_sampler_state_.GetAddressOf());

	for (auto& mesh : m_meshes_)
	{
		m_device_context_->IASetVertexBuffers(0, 1, mesh->m_vertex_buffer_.GetAddressOf(), &stride, &offset);
		m_device_context_->IASetIndexBuffer(mesh->m_index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
		m_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		vector<ID3D11Buffer*>	vertex_constant_buffers{ m_vertex_cbuffer_.Get(), m_camera_->m_vertex_cbuffer_.Get() };
		vector<ID3D11Buffer*>	pixel_constant_buffers = { m_pixel_cbuffer_.Get(), m_light_->m_pixel_cbuffer_.Get() };

		m_device_context_->VSSetConstantBuffers(0, (UINT)vertex_constant_buffers.size(), vertex_constant_buffers.data());
		m_device_context_->PSSetConstantBuffers(0, (UINT)pixel_constant_buffers.size(), pixel_constant_buffers.data());

		m_device_context_->PSSetShaderResources(0, 1, mesh->m_texture_sr_view_.GetAddressOf());

		m_device_context_->DrawIndexed(mesh->m_index_count_, 0, 0);
	}
}

void MeshGroup::Update()
{
	D3D11Utilizer::UpdateBuffer(m_device_, m_device_context_, m_vertex_constant_data_, m_vertex_cbuffer_);
	D3D11Utilizer::UpdateBuffer(m_device_, m_device_context_, m_pixel_constant_data_, m_pixel_cbuffer_);
}