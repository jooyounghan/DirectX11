#pragma once
#include "FilteredCamera.h"
#include "IDPickableRenderTarget.h"

class PickableCamera : public FilteredCamera, public IDPickableRenderTarget
{
	friend class CameraManipulator;

public:
	PickableCamera(
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
	);
	virtual ~PickableCamera();

protected:
	static size_t ullPickableCamaraCnt;
	size_t ullPickableCamaraIdx;

public:
	inline virtual std::tuple<UINT, std::vector<ID3D11RenderTargetView*>, ID3D11DepthStencilView*> GetRTVs() {
		return std::make_tuple<UINT, std::vector<ID3D11RenderTargetView*>>(
			2, 
			std::vector<ID3D11RenderTargetView*> { 
				RenderTarget::cpRTV.Get(),
				IDPickableRenderTarget::cpRTV.Get()
			},
			cpDSV.Get()
		);
	};

public:
	virtual size_t GetCameraID() override { return ullPickableCamaraIdx; };
	virtual void ClearRTV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraInformation(class CameraManipulator* pCameraManipulator) override;

public:
	virtual uint32_t GetPickedID() override;
};

