#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>

class IMovable
{
public:
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

protected:
	DirectX::XMVECTOR						xmvPosition;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpPositionBuffer;

public:
	inline DirectX::XMVECTOR& GetPosition() { return xmvPosition; }
	void UpdatePosition();
};

