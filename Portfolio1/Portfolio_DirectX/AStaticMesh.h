#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath/DirectXMath.h>
#include <wrl/client.h>

#include <vector>
#include "IModel.h"
#include "ATransformerable.h"

struct TextureCoord
{
	float x;
	float y;
};

struct Vertex
{
	float x;
	float y;
	float z;
};

struct NormalVector
{
	float x;
	float y;
	float z;
};

struct InputLayout
{
	Vertex vertex;
	TextureCoord texcoord;
	NormalVector normal;
};

class AStaticMesh : public IModel, public ATransformerable
{
public:
	AStaticMesh();
	virtual ~AStaticMesh();

protected:
	std::vector<InputLayout>	inputData;
	std::vector<uint32_t>		indexData;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> inputBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

public:
	virtual void Load(const std::string& path) = 0;
	virtual void Draw() = 0;
	virtual void Update(const float& fDelta) = 0;

public:
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) override final;
};

