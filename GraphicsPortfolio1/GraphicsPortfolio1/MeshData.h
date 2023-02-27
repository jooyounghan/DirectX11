#pragma once

#include <string>
#include <vector>
#include <directxtk/SimpleMath.h>

using std::string;
using std::vector;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

struct Vertex
{
	Vector3		position;
	Vector3		normal;
	Vector2		uv;
};

struct MeshData
{
public:
	std::vector<Vertex>		vertices;
	std::vector<uint32_t>	indices;
	std::string				base_texture_name;
};

struct ModelData
{
	vector<MeshData> mesh_data;
	string file_name;
	float model_translation[3]{ 0.f, 0.f, 0.f };
	float model_rotation[3]{ 0.f, 0.f, 0.f };
	float model_scaling[3]{ 1.f, 1.f, 1.f };
};