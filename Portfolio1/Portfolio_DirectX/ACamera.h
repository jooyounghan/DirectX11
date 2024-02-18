#pragma once

#include "ViewableRenderTarget.h"
#include "ViewableDepthStencil.h"
#include "AFilter.h"
#include "APossessable.h"

#include <tuple>
#include <vector>

class ACamera
	: public ViewableRenderTarget,
	public ViewableDepthStencil, 
	public AFilter,
	public APossessable
{
public:
	ACamera(
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
	virtual ~ACamera();

protected:
	bool isLinkedWithBackBuffer;

public:
	virtual size_t GetCameraID() = 0;
	virtual void Resolve() = 0;

public:
	virtual std::tuple<UINT, std::vector<ID3D11RenderTargetView*>, ID3D11DepthStencilView*> GetRTVs() = 0;

public:
	virtual void ManageKeyBoardInput(const float& fDelay);
	virtual void ManageMouseInput(const int& iMouseXIn, const int& iMouseYIn);

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	inline void LinkWithBackBuffer(bool isLinked) { isLinkedWithBackBuffer = isLinked; };

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override final;
	virtual void SetUAVBarrier() override final;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraList(class CameraManipulator* pCameraManipulator) = 0;
	virtual void AcceptCameraInformation(class CameraManipulator* pCameraManipulator) = 0;
};

