#pragma once
#include <vector>
#include <memory>

#include "Mesh.h"
#include "Shader.h"

using std::vector;
using std::shared_ptr;

class IMeshGroup
{
public:
	IMeshGroup();
	~IMeshGroup();

public:
	vector<shared_ptr<Mesh>>	m_meshes_;

	// 구조는 바뀔 수 있음
	shared_ptr<Shader>			m_mesh_shader_;

public:
	MeshGroupVertexConstantData		m_vertex_constant_data_;

	ComPtr<ID3D11Buffer> m_vertex_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_cbuffer_;

public:
	void Render(ComPtr<ID3D11DeviceContext>& device_context);
};

/*
현재 구조
1. 캐릭터는 IMeshGroup이고, 캐릭터의 팔, 다리 등은 Mesh로 관리한다.
2. Rendering 할 때, Mesh에 대해 기본적인 렌더링을 한다(머터리얼을 참고하여 색을 입히고, 트랜스폼한다.)
3. MeshGroup을 통해 최종적인 회전 등을 하고 빛을 렌더링한다. (캐릭터에 대한 빛은 동일 할 것이므로)

애니메이션을 어떻게 처리하는지 몰라서 위처럼 구조를 짜봤음
*/
