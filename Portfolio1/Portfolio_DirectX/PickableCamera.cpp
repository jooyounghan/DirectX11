#include "PickableCamera.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DirectXDevice.h"

using namespace std;

PickableCamera::PickableCamera(

	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: AFilteredCamera(
		fXPos, fYPos, fZPos,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	), IDPickableRenderTarget(uiWidthIn, uiHeightIn, uiNumQualityLevelsIn),
	ARectangle(uiWidthIn, uiHeightIn)
{

}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	AFilteredCamera::Resize(uiWidthIn, uiHeightIn);
	IDPickableRenderTarget::Resize(uiWidthIn, uiHeightIn);
}

DirectX::XMMATRIX PickableCamera::GetTranformMat()
{
	return MathematicalHelper::MakeAffineTransformation(
		1.f, 1.f, 1.f,
		sAngles.fPitch, sAngles.fYaw, sAngles.fRoll,
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);
}

uint32_t PickableCamera::GetPickedID()
{
	uint32_t uiResult = 0;
	ID3D11Texture2D* pReferenceIDTexture = IDPickableRenderTarget::ARenderTarget::cpTexture2D.Get();

	if (ARenderTarget::uiNumQualityLevels > 0)
	{
		IDPickableRenderTarget::Apply(IDPickableRenderTarget::ARenderTarget::cpSRV.GetAddressOf());
		pReferenceIDTexture = IDPickableRenderTarget::IFilter::cpTexture2D.Get();
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
