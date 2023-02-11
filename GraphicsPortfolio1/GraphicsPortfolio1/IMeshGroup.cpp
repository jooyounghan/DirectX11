#include "IMeshGroup.h"

IMeshGroup::IMeshGroup()
{
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

		device_context->VSSetConstantBuffers(0, 1, m_vertex_cbuffer_.GetAddressOf());
		//device_context->PSSetConstantBuffers(0, 1, mesh->m_pixel_cbuffer_.GetAddressOf());

		//device_context->PSSetShaderResources(0, 1, mesh->m_texture_sr_view_.GetAddressOf());

		device_context->DrawIndexed(mesh->m_index_count_, 0, 0);
	}
}