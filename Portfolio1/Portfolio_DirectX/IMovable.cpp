#include "IMovable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

const DirectX::XMVECTOR IMovable::xmvDefaultDirection = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
const DirectX::XMVECTOR IMovable::xmvDefaultUp = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
const DirectX::XMVECTOR IMovable::xmvDefaultRight = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);

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

void IMovable::SetPosition(const float& x, const float& y, const float& z)
{
	xmvPosition.m128_f32[0] = x;
	xmvPosition.m128_f32[1] = y;
	xmvPosition.m128_f32[2] = z;
}

void IMovable::UpdatePosition()
{
	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, xmvPosition, D3D11_MAP_WRITE_DISCARD, cpPositionBuffer.Get());
}
