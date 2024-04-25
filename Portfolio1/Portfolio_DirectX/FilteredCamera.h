#pragma once
#include "ACamera.h"
#include <vector>
#include <stack>
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

public:
	virtual void SetAsRenderTarget() override;
	virtual void ResetAsRenderTarget() override;

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

protected:
	std::stack<ID3D11ShaderResourceView**> stackBloomInputs;

public:
	inline void SetBloomInput(ID3D11ShaderResourceView** ppBloomInput) { stackBloomInputs.push(ppBloomInput); }
	inline const bool IsBloomInputSet() { return !stackBloomInputs.empty(); }
	void AddBloomState();
};

