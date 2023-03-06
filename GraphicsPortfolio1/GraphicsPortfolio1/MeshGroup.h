#pragma once
#include <memory>

#include "Mesh.h"
#include "Shader.h"
#include "Light.h"

class MeshGroup : public IRenderable
{
public:
	MeshGroup(ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11DeviceContext>& device_context,
		std::shared_ptr<IRenderable> camera,
		std::shared_ptr<IRenderable> light);
	~MeshGroup();

	
public:
	std::vector<std::shared_ptr<Mesh>>	m_meshes_;

public:
	MeshVertexConstantData		m_vertex_constant_data_;

public:
	void AddMeshData(const std::vector<MeshData>& mesh_data_vector);

public:
	void InitConstantData(ComPtr<ID3D11Device>& device);
	void SetVertexConstantData(const Matrix& model_tranform);

public:
	std::shared_ptr<IRenderable> m_camera_;
	std::shared_ptr<IRenderable> m_light_;

public:
	virtual void Render() override;
	virtual void Update() override;
};
