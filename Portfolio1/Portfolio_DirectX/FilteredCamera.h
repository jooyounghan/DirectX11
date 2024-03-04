#pragma once
#include "ACamera.h"
#include <vector>
#include <memory>

class FilteredCamera : public ACamera
{
	friend class CameraManipulator;

public:
	FilteredCamera(
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
	virtual ~FilteredCamera();

protected:
	static size_t ullFiltertedCamaraCnt;
	size_t ullFiltertedCamaraIdx;

protected:
	std::vector<std::unique_ptr<class AFilter>> upFilters;

private:
	static ID3D11RenderTargetView* pNullRTV;

public:
	virtual void SetCameraAsRenderTarget() override;
	virtual void ResetCameraAsRenderTarget() override;

public:
	inline const std::vector<std::unique_ptr<class AFilter>>& GetFilters() { return upFilters; }
	virtual size_t GetCameraID() override { return ullFiltertedCamaraIdx; };

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void Resolve() override;

public:
	virtual void AcceptCameraList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraInformation(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptLButtonDown(
		class CameraManipulator* pCameraManipulator,
		const int& xPosIn,
		const int& yPosIn
	) override;

public:
	void AddBlurState();
};

