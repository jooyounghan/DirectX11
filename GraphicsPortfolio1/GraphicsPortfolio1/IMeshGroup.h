#pragma once
#include <memory>

#include "Mesh.h"
#include "Shader.h"
#include "Light.h"

using std::shared_ptr;

class IMeshGroup
{
public:
	IMeshGroup(ComPtr<ID3D11Device>& device);
	IMeshGroup(
		ComPtr<ID3D11Device>& device,
		const vector<MeshData>& mesh_data_vector
	);
	~IMeshGroup();
public:
	vector<shared_ptr<Mesh>>	m_meshes_;

public:
	shared_ptr<Shader>			m_mesh_shader_;

public:
	MeshVertexConstantData		m_vertex_constant_data_;
	MaterialConstantData		m_material_constant_data_;

public:
	void InitConstantData(ComPtr<ID3D11Device>& device);
	void SetVertexConstantData(const Matrix& model_tranform);
	void SetMaterialConstantData(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float shininess);

public:
	ComPtr<ID3D11Buffer> m_vertex_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_cbuffer_;

public:
	void Render(ComPtr<ID3D11DeviceContext>& device_context,
		const ComPtr<ID3D11Buffer>& vertex_camera_cbuffer_,
		const ComPtr<ID3D11Buffer>& light_cbuffer);
	void Update(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
};
