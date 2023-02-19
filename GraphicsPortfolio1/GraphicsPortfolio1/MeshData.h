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