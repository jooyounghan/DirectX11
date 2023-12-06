#include "BloomCamera.h"
#include "PostProcess.h"

using namespace std;

BloomCamera::BloomCamera(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn,
	const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
	const float& fFovDegreeIn, const float& fNearZIn, const float& fFarZIn
)
	: NormalCamera(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn, uiNumLevelQuality, eCameraFormatIn,
		fPosXIn, fPosYIn, fPosZIn, fFovDegreeIn, fNearZIn, fFarZIn
	)
{
	upPostProcess->AddBloomFilter();
	upPostProcess->AddBloomFilter();
}

BloomCamera::~BloomCamera()
{
}
