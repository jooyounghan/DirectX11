#include "PickableCamera.h"
#include "MathematicalHelper.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "CameraManipulator.h"

using namespace std;

size_t PickableCamera::ullPickableCamaraCnt = 0;
vector<ID3D11RenderTargetView*> PickableCamera::pNullRTV = { nullptr, nullptr };

PickableCamera::PickableCamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovDegIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: FilteredCamera(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		uiWidthIn, uiHeightIn,
		fFovDegIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	), 
	IDPickableRenderTarget(
		uiWidthIn, uiHeightIn, 
		uiNumQualityLevelsIn
	),
	Viewable(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)uiWidthIn, 
		(float)uiHeightIn, 
		fFovDegIn,
		fNearZIn, fFarZIn
	),
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IRectangle(
		uiWidthIn,
		uiHeightIn
	)
{
	ullPickableCamaraCnt++;
	ullFiltertedCamaraCnt--;
	ullPickableCamaraIdx = ullPickableCamaraCnt;
}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::SetCameraAsRenderTarget()
{
	std::vector<ID3D11RenderTargetView*> vRTVs = {
		RenderTarget::cpRTV.Get(),
		IDPickableRenderTarget::cpRTV.Get()
	};
	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vRTVs.data(), cpDSV.Get());
	DirectXDevice::pDeviceContext->RSSetViewports(1, &sViewPort);
}

void PickableCamera::ResetCameraAsRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, pNullRTV.data(), nullptr);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &nullViewPort);
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

void PickableCamera::AcceptFilterList(CameraManipulator* pCameraManipulator)
{
	ACamera::AcceptFilterList(pCameraManipulator);
	pCameraManipulator->VisitFilterList(*this);
}

void PickableCamera::AcceptCameraList(CameraManipulator* pCameraManipulator)
{
	pCameraManipulator->VisitCameraList(*this);
}

void PickableCamera::AcceptCameraInformation(CameraManipulator* pCameraManipulator)
{
	pCameraManipulator->VisitCameraInfo(*this);
}

void PickableCamera::AcceptLButtonDown(
	CameraManipulator* pCameraManipulator,
	const int& xPosIn,
	const int& yPosIn
)
{
	pCameraManipulator->VisitLButtonDown(*this, xPosIn, yPosIn);
}

uint32_t PickableCamera::GetPickedID()
{
	uint32_t uiResult = 0;
	ID3D11Texture2D* pReferenceIDTexture = IDPickableRenderTarget::RenderTarget::cpTexture2D.Get();

	if (RenderTarget::uiNumQualityLevels > 0)
	{
		IDPickableRenderTarget::Apply(IDPickableRenderTarget::RenderTarget::GetAddressOfSRV());
		pReferenceIDTexture = IDPickableRenderTarget::AFilter::cpTexture2D.Get();
	}

	D3D11_BOX sBox;
	AutoZeroMemory(sBox);
	sBox.left = sMousePosPixel.uiMouseXPixel;
	sBox.right = sBox.left + 1;
	sBox.top = sMousePosPixel.uiMouseYPixel;
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
