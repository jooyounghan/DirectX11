#include "PickableCamera.h"
#include "MathematicalHelper.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

using namespace std;

PickableCamera::PickableCamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: FilteredCamera(
		fXPos, fYPos, fZPos,
		fPitchRadIn, fYawRadIn, fRollRadIn,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	), 
	IDPickableRenderTarget(
		uiWidthIn, uiHeightIn, 
		uiNumQualityLevelsIn
	),
	Viewable(
		fXPos, fYPos, fZPos,
		fPitchRadIn, fYawRadIn, fRollRadIn,
		(float)uiWidthIn, 
		(float)uiHeightIn, 
		fFovRadIn, 
		fNearZIn, fFarZIn
	),
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchRadIn, fYawRadIn, fRollRadIn),
	IRectangle(
		uiWidthIn,
		uiHeightIn
	)
{

}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::ClearRTV()
{
	FilteredCamera::ClearRTV();
	IDPickableRenderTarget::ClearRTV();
}

void PickableCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	FilteredCamera::Resize(uiWidthIn, uiHeightIn);
	IDPickableRenderTarget::Resize(uiWidthIn, uiHeightIn);
}

uint32_t PickableCamera::GetPickedID()
{
	uint32_t uiResult = 0;
	ID3D11Texture2D* pReferenceIDTexture = IDPickableRenderTarget::RenderTarget::cpTexture2D.Get();

	if (RenderTarget::uiNumQualityLevels > 0)
	{
		IDPickableRenderTarget::Apply(IDPickableRenderTarget::RenderTarget::cpSRV.GetAddressOf());
		pReferenceIDTexture = IDPickableRenderTarget::AFilter::cpTexture2D.Get();
	}

	D3D11_BOX sBox;
	AutoZeroMemory(sBox);
	sBox.left = sMousePosNdc.uiMouseXNdc;
	sBox.right = sBox.left + 1;
	sBox.top = sMousePosNdc.uiMouseYNdc;
	sBox.bottom = sBox.top + 1;
	sBox.front = 0;
	sBox.back = 1;

	DirectXDevice::pDeviceContext->CopySubresourceRegion(cpPickedIDResult.Get(), 0, 0, 0, 0, pReferenceIDTexture, 0, &sBox);

	D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
	AutoZeroMemory(sMappedSubResource);
	DirectXDevice::pDeviceContext->Map(cpPickedIDResult.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
	memcpy(&uiResult, sMappedSubResource.pData, sizeof(uint32_t));
	DirectXDevice::pDeviceContext->Unmap(cpPickedIDResult.Get(), 0);

	return uiResult;
}
