#pragma once
#include <memory>

#include "Mesh.h"
#include "Shader.h"
#include "Light.h"

using std::shared_ptr;

class IMeshGroup
{
public:
	IMeshGroup();
	IMeshGroup(
		ComPtr<ID3D11Device>& device,
		const vector<MeshData>& mesh_data_vector
	);
	~IMeshGroup();

public:
	vector<shared_ptr<Mesh>>	m_meshes_;

	// 구조는 바뀔 수 있음
	shared_ptr<Shader>			m_mesh_shader_;

public:
	void Render(ComPtr<ID3D11DeviceContext>& device_context,
		const ComPtr<ID3D11Buffer>& vertex_camera_cbuffer_,
		const ComPtr<ID3D11Buffer>& light_cbuffer);
	void Update(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
};
