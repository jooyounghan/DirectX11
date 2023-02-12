#include <iostream>

#include "Mesh.h"
#include "D3D11Utilizer.h"
#include "FileReader.h"

using namespace std;

Mesh::Mesh(ComPtr<ID3D11Device>& device, const MeshData& mesh_data)
{
	D3D11Utilizer::CreateVertexBuffer<Vertex>(device, mesh_data.vertices, m_vertex_buffer_);
	D3D11Utilizer::CreateIndexBuffer<uint32_t>(device, mesh_data.indices, m_index_buffer_, m_index_count_);
	const Image& texture_image = FileReader::GetImage(mesh_data.base_texture_name);
	D3D11Utilizer::CreateTexture(device, texture_image, m_texture_, m_texture_sr_view_);
}

Mesh::~Mesh()
{

}
