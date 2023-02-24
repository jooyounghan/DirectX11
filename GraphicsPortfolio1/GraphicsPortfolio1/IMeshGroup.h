#pragma once
#include <memory>

#include "Mesh.h"
#include "Shader.h"

using std::shared_ptr;

class IMeshGroup
{
public:
	IMeshGroup(
		ComPtr<ID3D11Buffer>& vertex_stage_cbuffer,
		ComPtr<ID3D11Buffer>& pixel_stage_cbuffer
	);
	IMeshGroup(
		ComPtr<ID3D11Device>& device,
		const vector<MeshData>& mesh_data_vector,
		ComPtr<ID3D11Buffer>& vertex_stage_cbuffer,
		ComPtr<ID3D11Buffer>& pixel_stage_cbuffer
	);
	~IMeshGroup();

public:
	vector<shared_ptr<Mesh>>	m_meshes_;

	// 구조는 바뀔 수 있음
	shared_ptr<Shader>			m_mesh_shader_;

public:
	ComPtr<ID3D11Buffer>& m_vertex_stage_cbuffer_;
	ComPtr<ID3D11Buffer>& m_pixel_stage_cbuffer_;

public:
	void Render(ComPtr<ID3D11DeviceContext>& device_context);
	void Update(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
};

/*
현재 구조
1. 캐릭터는 IMeshGroup이고, 캐릭터의 팔, 다리 등은 Mesh로 관리한다.
2. Rendering 할 때, Mesh에 대해 기본적인 렌더링을 한다(머터리얼을 참고하여 색을 입히고, 트랜스폼한다.)
3. MeshGroup을 통해 최종적인 회전 등을 하고 빛을 렌더링한다. (캐릭터에 대한 빛은 동일 할 것이므로)

애니메이션을 어떻게 처리하는지 몰라서 위처럼 구조를 짜봤음
*/
