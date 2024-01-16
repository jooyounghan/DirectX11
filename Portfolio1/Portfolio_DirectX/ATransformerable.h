#pragma once
#include "IMovable.h"
#include "IScalable.h"
#include "IAngleAdjustable.h"

class ATransformerable : public IMovable, public IScalable, public IAngleAdjustable
{
public:
	ATransformerable();
	virtual ~ATransformerable();

protected:
	struct
	{
		DirectX::XMMATRIX xmmTransformation;
		DirectX::XMMATRIX xmmInvTransformation;
	} sTransformation;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTransformationBuffer;

public:
	virtual void Update(const float& fDelta) = 0;

public:
	void UpdateTranformMat();
};

