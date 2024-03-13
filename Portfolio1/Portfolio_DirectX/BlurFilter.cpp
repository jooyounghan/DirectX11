#include "BlurFilter.h"

#include "BlurComputeShader.h"

#include "DirectXDevice.h"
#include "CameraManipulator.h"

BlurFilter::BlurFilter(
	UINT uiWidthIn, 
	UINT uiHeightIn, 
	UINT uiArraySizeIn, 
	UINT uiNumQualityLevelsIn, 
	UINT uiBindFlagIn, 
	UINT uiCPUAccessIn, 
	UINT uiMiscFlagIn, 
	D3D11_USAGE eUsageIn, 
	DXGI_FORMAT eFormatIn
)
	: AFilter(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn,
		eFormatIn,
		256, 1, 1
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	pBlurCS = BlurComputeShader::GetInstance();
}

BlurFilter::~BlurFilter()
{
}

void BlurFilter::AcceptFilterList(CameraManipulator* pCameraManipulator)
{
	pCameraManipulator->VisitFilterList(*this);
}

void BlurFilter::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	pBlurCS->ApplyShader();
	pBlurCS->SetShader(ppInputSRV, cpUAV.GetAddressOf());

	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ);
	pBlurCS->ResetShader();
	pBlurCS->DisapplyShader();
}