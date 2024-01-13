#pragma once
#include "IDirectXDevice.h"
#include <wrl/client.h>

class IMovable : virtual public IDirectXDevice
{
public:
	IMovable(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn
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

