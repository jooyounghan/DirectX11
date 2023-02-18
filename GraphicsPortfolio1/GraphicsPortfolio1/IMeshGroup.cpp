#include "IMeshGroup.h"

using namespace std;

IMeshGroup::IMeshGroup()
{
}

IMeshGroup::IMeshGroup(ComPtr<ID3D11Device>& device, const vector<MeshData>& mesh_data_vector)
{
	for (auto& mesh_data : mesh_data_vector)
	{
		shared_ptr<Mesh> mesh = std::make_shared<Mesh>(device, mesh_data);
		m_meshes_.push_back(mesh);
	}
}

IMeshGroup::~IMeshGroup()
{
}

void IMeshGroup::Render(ComPtr<ID3D11DeviceContext>& device_context)
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

		vector<ID3D11Buffer*> vertex_constant_buffers{ mesh->m_vertex_cbuffer_.Get(), m_vertex_stage_cbuffer_.Get() };
		//vector<ID3D11Buffer**> pixel_constant_buffers{ mesh->m_vertex_cbuffer_.GetAddressOf(), m_vertex_cbuffer_.GetAddressOf() };

		device_context->VSSetConstantBuffers(0, vertex_constant_buffers.size(), vertex_constant_buffers.data());
		//device_context->PSSetConstantBuffers(0, 1, mesh->m_pixel_cbuffer_.GetAddressOf());

		device_context->PSSetShaderResources(0, 1, mesh->m_texture_sr_view_.GetAddressOf());

		device_context->DrawIndexed(mesh->m_index_count_, 0, 0);
	}
}