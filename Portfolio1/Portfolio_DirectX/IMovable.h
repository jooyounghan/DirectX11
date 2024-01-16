#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class IMovable
{
public:
	IMovable();
	IMovable(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos
	);
	virtual ~IMovable();

protected:
	static const DirectX::XMVECTOR			xmvDefaultDirection;
	static const DirectX::XMVECTOR			xmvDefaultUp;
	static const DirectX::XMVECTOR			xmvDefaultRight;

public:
	DirectX::XMVECTOR						xmvPosition;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpPositionBuffer;
};

