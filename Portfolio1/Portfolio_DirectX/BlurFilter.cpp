#include "BlurFilter.h"

#include "ColorBlurComputeShader.h"

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
		32, 32, 1
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	pBlurCS = ColorBlurComputeShader::GetInstance();
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
	SingleSourceCSBindingSet sBinding;
	sBinding.ppInputSRV = ppInputSRV;
	sBinding.ppOutputUAV = cpUAV.GetAddressOf();

	pBlurCS->ApplyShader();
	pBlurCS->SetShader(&sBinding);

	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ);
	pBlurCS->ResetShader();
	pBlurCS->DisapplyShader();
}