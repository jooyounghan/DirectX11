#include "BloomFilter.h"

#include "InputMergeComputeShader.h"

#include "DirectXDevice.h"
#include "CameraManipulator.h"

BloomFilter::BloomFilter(UINT uiWidthIn, UINT uiHeightIn,
	UINT uiArraySizeIn,
	UINT uiNumQualityLevelsIn,
	UINT uiBindFlagIn,
	UINT uiCPUAccessIn,
	UINT uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn
)
	: ADualInputFilter(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn,
		eFormatIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	pBloomCS = InputMergeComputeShader::GetInstance();
}

BloomFilter::~BloomFilter()
{

}

void BloomFilter::AcceptFilterList(CameraManipulator* pCameraManipulator)
{
	pCameraManipulator->VisitFilterList(*this);
}

void BloomFilter::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	DoubleSourceCSBindingSet sBinding;
	sBinding.ppInputSRV1 = ppPresetInputSRV;
	sBinding.ppInputSRV2 = ppInputSRV;
	sBinding.ppOutputUAV = cpUAV.GetAddressOf();

	pBloomCS->ApplyShader();
	pBloomCS->SetShader(&sBinding);

	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ);
	pBloomCS->ResetShader();
	pBloomCS->DisapplyShader();

}
