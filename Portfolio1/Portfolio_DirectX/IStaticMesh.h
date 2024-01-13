#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath/DirectXMath.h>
#include <wrl/client.h>

#include <vector>
#include "IModel.h"
#include "IMovable.h"
#include "IScalable.h"

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

class IStaticMesh : public IModel, public IMovable, public IScalable
{
public:
	IStaticMesh(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~IStaticMesh();

protected:
	std::vector<InputLayout>	inputData;
	std::vector<uint32_t>		indexData;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> inputBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

public:
	virtual void Load(const std::string& path) = 0;
	virtual void Render() = 0;
	virtual void Update(const float& fDelta) = 0;
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

