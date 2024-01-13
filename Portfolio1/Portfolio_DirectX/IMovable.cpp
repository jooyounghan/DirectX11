#include "IMovable.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"

using namespace DirectX;

const XMVECTOR IMovable::xmvDefaultDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
const XMVECTOR IMovable::xmvDefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
const XMVECTOR IMovable::xmvDefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

IMovable::IMovable(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: IDirectXDevice(pDeviceIn, pDeviceContextIn),
	xmvPosition(XMVectorSet(0.f, 0.f, 0.f, 1.f))
{
	AutoZeroMemory(sAngles);
	ID3D11Helper::CreateBuffer(
		pDevice, xmvPosition, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, 
		NULL, cpPositionBuffer.GetAddressOf()
	);
}

IMovable::~IMovable()
{
}
