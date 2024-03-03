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
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTransformationBuffer;

public:
	inline ID3D11Buffer* const* GetTransformationBuffer() { return cpTransformationBuffer.GetAddressOf(); }

public:
	void UpdateTranformationMatrix();
};

