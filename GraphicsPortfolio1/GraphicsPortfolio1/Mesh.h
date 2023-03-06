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

class Mesh
{
public:
	Mesh(ComPtr<ID3D11Device>& device, const MeshData& mesh_data);
	~Mesh();

public:
	ComPtr<ID3D11Buffer> m_vertex_buffer_;
	ComPtr<ID3D11Buffer> m_index_buffer_;

public:
	unsigned int m_index_count_ = 0;

	ComPtr<ID3D11Texture2D> m_texture_;
	ComPtr<ID3D11ShaderResourceView> m_texture_sr_view_;


};
