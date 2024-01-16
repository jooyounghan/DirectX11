#include "IMovable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

using namespace DirectX;

const XMVECTOR IMovable::xmvDefaultDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
const XMVECTOR IMovable::xmvDefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
const XMVECTOR IMovable::xmvDefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

IMovable::IMovable()
	: xmvPosition(XMVectorSet(0.f, 0.f, 0.f, 1.f))
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, xmvPosition,
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, 
		NULL, cpPositionBuffer.GetAddressOf()
	);
}

IMovable::IMovable(const float& fXPos, const float& fYPos, const float& fZPos)
{
	xmvPosition.m128_f32[0] = fXPos;
	xmvPosition.m128_f32[1] = fYPos;
	xmvPosition.m128_f32[2] = fZPos;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, xmvPosition,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		NULL, cpPositionBuffer.GetAddressOf()
	);
}

IMovable::~IMovable()
{
}
