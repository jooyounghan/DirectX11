#include "Viewable.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DirectXDevice.h"

using namespace DirectX;

Viewable::Viewable(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const float& fWidthIn,
	const float& fHeightIn,
	const float& fFovDegreeIn,
	const float& fNearZIn,
	const float& fFarZIn
)
	: IMovable(fXPos, fYPos, fZPos), 
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IRectangle((UINT)fWidthIn, (UINT)fHeightIn),
	fFovDegree(fFovDegreeIn), fNearZ(fNearZIn), fFarZ(fFarZIn)
{
	AutoZeroMemory(sViewPort);
	
	sViewPort.TopLeftX = 0.f;
	sViewPort.TopLeftY = 0.f;
	sViewPort.Width = fWidthIn;
	sViewPort.Height = fHeightIn;
	sViewPort.MinDepth = 0.f;
	sViewPort.MaxDepth = 1.f;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sViewProjs,
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, NULL, 
		cpViewProjBuffer.GetAddressOf()
	);
}

Viewable::~Viewable()
{
}

void Viewable::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;
	sViewPort.Width = (float)uiWidth;
	sViewPort.Height= (float)uiHeight;
}

void Viewable::UpdateView()
{
	XMMATRIX xmRotationMat = GetTransformedMatrix();
	XMVECTOR xmvDirection = XMVector4Transform(xmvDefaultDirection, xmRotationMat);
	XMVECTOR xmvUp = XMVector4Transform(xmvDefaultUp, xmRotationMat);

	sViewProjs.xmmViewProjMat = MathematicalHelper::MakeViewProjMatrix(
		xmvPosition,
		xmvDirection,
		xmvUp,
		DirectX::XMConvertToRadians(fFovDegree), sViewPort.Width / sViewPort.Height,
		fNearZ, fFarZ
	);

	sViewProjs.xmmInvViewProjMat = XMMatrixInverse(nullptr, sViewProjs.xmmViewProjMat);
	sViewProjs.xmmViewProjMat = XMMatrixTranspose(sViewProjs.xmmViewProjMat);

	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sViewProjs, D3D11_MAP_WRITE_DISCARD, cpViewProjBuffer.Get());
}
