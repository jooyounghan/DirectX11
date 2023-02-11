#include "Stage.h"

#include <memory>
#include <DirectXMath.h>
#include "D3D11Utilizer.h"
#include "BaseMeshShader.h"

using namespace std;

using namespace DirectX;

Stage::Stage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context)
	: m_device_(device), m_device_context_(device_context)
{
	/*
	Test
	*/
	std::vector<Vertex> vertices
	{
		{{-0.001, 0.001, -1}, {0, 0, 1}, {0,0}},
		{{0.001, 0.001, -1}, {0, 0, 1}, {1,0}},
		{{0.001, -0.001, -1}, {0, 0, 1}, {0.5,1}},
		{{-0.001, -0.001, -1}, {0, 0, 1}, {0,1}},
		{{0.002, -0.001, -1}, {0, 0, 1}, {1,1}},
	};

	std::vector<uint32_t> indices{ 0, 2, 1, 0, 3, 2, 1, 2, 4 };

	MeshData testData;
	testData.vertices = vertices;
	testData.indices = indices;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();

	D3D11Utilizer::CreateVertexBuffer<Vertex>(m_device_, vertices, mesh->m_vertex_buffer_);
	D3D11Utilizer::CreateIndexBuffer<uint32_t>(m_device_, indices, mesh->m_index_buffer_);
	mesh->m_index_count_ = indices.size();

	shared_ptr<IMeshGroup> i_mesh_group = make_shared<IMeshGroup>();
	i_mesh_group->m_mesh_shader_ = make_shared<BaseMeshShader>(m_device_, L"BaseVertexShader.hlsl", L"BasePixelShader.hlsl");
	i_mesh_group->m_meshes_.push_back(mesh);

	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;
	float aspect = 1920.f / 1080.f;

	i_mesh_group->m_vertex_constant_data_.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), aspect, m_nearZ, m_farZ);

	D3D11Utilizer::CreateConstantBuffer<MeshGroupVertexConstantData>(m_device_, i_mesh_group->m_vertex_constant_data_, i_mesh_group->m_vertex_cbuffer_);

	m_mesh_group.push_back(i_mesh_group);
}

Stage::~Stage()
{

}

inline void Stage::AddMeshGroup(shared_ptr<IMeshGroup> mesh_group)
{
	m_mesh_group.push_back(mesh_group);
}

inline void Stage::RemoveMeshGroup(const size_t& index)
{
	m_mesh_group.erase(m_mesh_group.begin() + index);
}

void Stage::Render()
{
	for (auto& mesh : m_mesh_group)
	{
		mesh->Render(m_device_context_);
	}
}
