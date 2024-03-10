#pragma once
#include "IMovable.h"
#include "IScalable.h"
#include "IAngleAdjustable.h"

class ATransformerable : virtual public IMovable, virtual public IScalable, virtual public IAngleAdjustable
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

