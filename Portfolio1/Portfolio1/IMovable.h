#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class IMovable
{
public:
	IMovable(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn
	);
	~IMovable();

protected:
	static const DirectX::XMVECTOR			xmvDefaultDirection;
	static const DirectX::XMVECTOR			xmvDefaultUp;
	static const DirectX::XMVECTOR			xmvDefaultRight;

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

public:
	DirectX::XMVECTOR						xmvPosition;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpPositionBuffer;

public:
	struct
	{
		float fPitch;
		float fYaw;
		float fRoll;
	} sAngles;

public:
	virtual void Update(const float& fDelta) = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

