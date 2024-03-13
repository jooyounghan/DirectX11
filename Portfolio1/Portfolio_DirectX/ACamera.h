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

protected:
	class TypeResolveComputeShader* pTypeResolveCS;
	class MS16ToSS8ComputeShader* pMS16ToSS8CS;

public:
	virtual size_t GetCameraID() = 0;
	virtual void Resolve() = 0;

public:
	virtual void ManageKeyBoardInput(const float& fDelay);
	virtual void ManageMouseInput(const int& iMouseXIn, const int& iMouseYIn);

public:
	virtual void SetAsRenderTarget() = 0;
	virtual void ResetAsRenderTarget() = 0;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	inline void LinkWithBackBuffer(bool isLinked) { isLinkedWithBackBuffer = isLinked; };

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override final;

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) override;
	virtual void AcceptCameraList(class CameraManipulator* pCameraManipulator) = 0;
	virtual void AcceptCameraInformation(class CameraManipulator* pCameraManipulator) = 0;

public:
	virtual void AcceptLButtonDown(
		class CameraManipulator* pCameraManipulator,
		const int& xPosIn,
		const int& yPosIn
	) = 0;
};

