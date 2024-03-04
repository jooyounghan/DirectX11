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
	virtual void SetCameraAsRenderTarget() override;
	virtual void ResetCameraAsRenderTarget() override;

private:
	static std::vector<ID3D11RenderTargetView*> pNullRTV;

public:
	virtual size_t GetCameraID() override { return ullPickableCamaraIdx; };
	virtual void ClearRTV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraInformation(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptLButtonDown(
		class CameraManipulator* pCameraManipulator,
		const int& xPosIn,
		const int& yPosIn
	) override;

public:
	virtual uint32_t GetPickedID() override;
};

