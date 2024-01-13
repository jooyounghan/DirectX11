#include "AViewable.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace DirectX;

AViewable::AViewable(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const float& fWidthIn,
	const float& fHeightIn,
	const float& fFovRadianIn,
	const float& fNearZIn,
	const float& fFarZIn
)
	: IMovable(pDeviceIn, pDeviceContextIn), fFovRadian(fFovRadianIn), fNearZ(fNearZIn), fFarZ(fFarZIn)
{
	AutoZeroMemory(sViewPort);
	
	sViewPort.TopLeftX = 0.f;
	sViewPort.TopLeftY = 0.f;
	sViewPort.Width = fWidthIn;
	sViewPort.Height = fHeightIn;
	sViewPort.MinDepth = 0.f;
	sViewPort.MaxDepth = 1.f;

	ID3D11Helper::CreateBuffer(
		pDevice, sViewProjs, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, NULL, 
		cpViewProjBuffer.GetAddressOf()
	);
}

AViewable::~AViewable()
{
}

void AViewable::UpdateViewProj()
{
	XMMATRIX xmRotationMat = XMMatrixRotationRollPitchYaw(sAngles.fPitch, sAngles.fYaw, sAngles.fRoll);

	XMVECTOR&& xmvDirection = XMVector4Transform(xmvDefaultDirection, xmRotationMat);
	XMVECTOR&& xmvUp = XMVector4Transform(xmvDefaultUp, xmRotationMat);

	sViewProjs.xmmViewProjMat = MathematicalHelper::MakeViewProjMatrix(
		xmvPosition, 
		std::move(xmvDirection),
		std::move(xmvUp),
		fFovRadian, sViewPort.Width / sViewPort.Height,
		fNearZ, fFarZ
	);

	sViewProjs.xmmInvViewProjMat = XMMatrixInverse(nullptr, sViewProjs.xmmViewProjMat);
	sViewProjs.xmmViewProjMat = XMMatrixTranspose(sViewProjs.xmmViewProjMat);

	ID3D11Helper::UpdateBuffer(pDeviceContext, sViewProjs, D3D11_MAP_WRITE_DISCARD, cpViewProjBuffer.Get());
}