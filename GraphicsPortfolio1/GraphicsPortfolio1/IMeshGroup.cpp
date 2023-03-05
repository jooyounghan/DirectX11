#include "IMeshGroup.h"
#include "D3D11Utilizer.h"

using namespace std;

IMeshGroup::IMeshGroup(ComPtr<ID3D11Device>& device)
{
	InitConstantData(device);
}

IMeshGroup::IMeshGroup(ComPtr<ID3D11Device>& device, const vector<MeshData>& mesh_data_vector)
{
	InitConstantData(device);
	for (auto& mesh_data : mesh_data_vector)
	{
		shared_ptr<Mesh> mesh = std::make_shared<Mesh>(device, mesh_data);
		m_meshes_.push_back(mesh);
	}
}

IMeshGroup::~IMeshGroup()
{
}

void IMeshGroup::InitConstantData(ComPtr<ID3D11Device>& device)
{
	m_vertex_constant_data_.model = Matrix::Identity;
	m_vertex_constant_data_.inv_tranpose = Matrix::Identity;

	m_material_constant_data_.ambient = Vector3(0.0f);
	m_material_constant_data_.diffuse = Vector3(0.0f);
	m_material_constant_data_.specular = Vector3(0.0f);
	m_material_constant_data_.shininess = 0.f;
	m_material_constant_data_.dummy1 = 0.f;
	m_material_constant_data_.dummy2 = 0.f;

	D3D11Utilizer::CreateConstantBuffer<MeshVertexConstantData>(device, m_vertex_constant_data_, m_vertex_cbuffer_);
	D3D11Utilizer::CreateConstantBuffer<MaterialConstantData>(device, m_material_constant_data_, m_pixel_cbuffer_);
}

void IMeshGroup::SetVertexConstantData(const Matrix& model_tranform)
{
	m_vertex_constant_data_.model = model_tranform.Transpose();
	m_vertex_constant_data_.inv_tranpose = m_vertex_constant_data_.model.Invert().Transpose();
}

void IMeshGroup::SetMaterialConstantData(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float shininess)
{
	m_material_constant_data_.ambient = ambient;
	m_material_constant_data_.diffuse = diffuse;
	m_material_constant_data_.specular = specular;
	m_material_constant_data_.shininess = shininess;
}

void IMeshGroup::Render(ComPtr<ID3D11DeviceContext>& device_context, 
	const ComPtr<ID3D11Buffer>& vertex_camera_cbuffer_,
	const ComPtr<ID3D11Buffer>& light_cbuffer)
{
	device_context->IASetInputLayout(m_mesh_shader_->m_input_layout_.Get());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	device_context->VSSetShader(m_mesh_shader_->m_vertex_shader_.Get(), 0, 0);
	device_context->PSSetShader(m_mesh_shader_->m_pixel_shader_.Get(), 0, 0);
	device_context->PSSetSamplers(0, 1, m_mesh_shader_->m_sampler_state_.GetAddressOf());

	for (auto& mesh : m_meshes_)
	{
		device_context->IASetVertexBuffers(0, 1, mesh->m_vertex_buffer_.GetAddressOf(), &stride, &offset);
		device_context->IASetIndexBuffer(mesh->m_index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		vector<ID3D11Buffer*>	vertex_constant_buffers{ m_vertex_cbuffer_.Get(), vertex_camera_cbuffer_.Get() };
		vector<ID3D11Buffer*>	pixel_constant_buffers = { light_cbuffer.Get() };

		device_context->VSSetConstantBuffers(0, (UINT)vertex_constant_buffers.size(), vertex_constant_buffers.data());
		device_context->PSSetConstantBuffers(0, (UINT)pixel_constant_buffers.size(), pixel_constant_buffers.data());

		device_context->PSSetShaderResources(0, 1, mesh->m_texture_sr_view_.GetAddressOf());

		device_context->DrawIndexed(mesh->m_index_count_, 0, 0);
	}
}

void IMeshGroup::Update(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	D3D11Utilizer::UpdateBuffer(device, context, m_vertex_constant_data_, m_vertex_cbuffer_);
	D3D11Utilizer::UpdateBuffer(device, context, m_material_constant_data_, m_pixel_cbuffer_);
}