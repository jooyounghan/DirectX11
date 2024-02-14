#pragma once
#include "ACamera.h"
#include <vector>
#include <memory>

class FilteredCamera : public ACamera
{
public:
	FilteredCamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn,
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

public:
	inline virtual std::tuple<UINT, std::vector<ID3D11RenderTargetView*>, ID3D11DepthStencilView*> GetRTVs() {
		return std::make_tuple<UINT, std::vector<ID3D11RenderTargetView*>>(
			1, std::vector<ID3D11RenderTargetView*>{ cpRTV.Get() }, cpDSV.Get()
		);
	};

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void Resolve() override;

public:
	virtual std::string GetCameraName() { return "Filtering Camera " + std::to_string(ullFiltertedCamaraIdx); };

public:
	void AddBlurState();
};

