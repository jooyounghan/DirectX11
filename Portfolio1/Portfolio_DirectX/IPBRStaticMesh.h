#pragma once
#include "IStaticMesh.h"

class IPBRStaticMesh : public IStaticMesh
{
public:
	IPBRStaticMesh(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~IPBRStaticMesh();

public:


public:
	virtual void Load(const std::string& path) = 0;
	virtual void Render() = 0;
	virtual void Update(const float& fDelta) = 0;
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

