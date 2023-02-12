#pragma once
#include <d3d11.h>
#include <windows.h>
#include <wrl.h>

#include "MeshData.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;


struct MeshVertexConstantData
{
	Matrix model;					// 16
	Matrix inv_tranpose;			// 16
};

struct MeshGroupVertexConstantData
{
	Matrix view;					// 16
	Matrix projection;			// 16
};

struct MaterialConstantData
{	
	Vector3 ambient = Vector3(0.0f);  // 12
	Vector3 diffuse = Vector3(0.0f);  // 12
	Vector3 specular = Vector3(1.0f); // 12
	float shininess = 0.01f;          // 4
	float dummy1;                     // 4
	float dummy2;                     // 4
};

class Mesh
{
public:
	Mesh();
	Mesh(ComPtr<ID3D11Device>& device, const MeshData& mesh_data);
	~Mesh();

public:
	MeshVertexConstantData		m_vertex_constant_data_;
	MaterialConstantData		m_material_constant_data_;

	ComPtr<ID3D11Buffer> m_vertex_buffer_;
	ComPtr<ID3D11Buffer> m_index_buffer_;

public:
	unsigned int m_index_count_ = 0;

public:
	ComPtr<ID3D11Buffer> m_vertex_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_cbuffer_;

	ComPtr<ID3D11Texture2D> m_texture_;
	ComPtr<ID3D11ShaderResourceView> m_texture_sr_view_;

public:
	void InitConstantData();
	void SetVertexConstantData(const Matrix& model_tranform);
	void SetMaterialConstantData(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float shininess);

public:
	void UpdateMesh(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const float& dt);
};
