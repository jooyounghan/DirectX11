#include <iostream>

#include "Mesh.h"
#include "D3D11Utilizer.h"
#include "FileReader.h"

using namespace std;

Mesh::Mesh(ComPtr<ID3D11Device>& device, const MeshData& mesh_data)
{
	InitConstantData(device);

	D3D11Utilizer::CreateVertexBuffer<Vertex>(device, mesh_data.vertices, m_vertex_buffer_);
	D3D11Utilizer::CreateIndexBuffer<uint32_t>(device, mesh_data.indices, m_index_buffer_, m_index_count_);

	Image image;
	FileReader::GetImage(mesh_data.base_texture_name, image);

	D3D11Utilizer::CreateTexture(device, image, m_texture_, m_texture_sr_view_);
}

Mesh::~Mesh()
{

}

void Mesh::InitConstantData(ComPtr<ID3D11Device>& device)
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

void Mesh::SetVertexConstantData(const Matrix& model_tranform)
{
	m_vertex_constant_data_.model = model_tranform.Transpose();
	m_vertex_constant_data_.inv_tranpose = m_vertex_constant_data_.model.Invert().Transpose();
}

void Mesh::SetMaterialConstantData(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float shininess)
{
	m_material_constant_data_.ambient = ambient;
	m_material_constant_data_.diffuse = diffuse;
	m_material_constant_data_.specular = specular;
	m_material_constant_data_.shininess = shininess;
}

void Mesh::UpdateMesh(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	D3D11Utilizer::UpdateBuffer(device, context, m_vertex_constant_data_, m_vertex_cbuffer_);
	D3D11Utilizer::UpdateBuffer(device, context, m_material_constant_data_, m_pixel_cbuffer_);
}


