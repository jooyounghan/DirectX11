#include "Stage.h"


Stage::Stage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context)
	: m_device_(device), m_device_context_(device_context)
{

}

Stage::~Stage()
{

}

void Stage::AddMeshGroup(shared_ptr<IMeshGroup> mesh_group)
{
	m_mesh_group.push_back(mesh_group);
}

void Stage::RemoveMeshGroup(const size_t& index)
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
