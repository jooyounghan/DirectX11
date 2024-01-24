#include "ACamera.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "Shaders.h"

ACamera::ACamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: ARenderTarget(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	ADepthStencil(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL, NULL, NULL,
		D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	IFilter(256, 1, 1),
	ASwapChainAccessable(),
	AViewable(
		fXPos, fYPos, fZPos,
		(float)uiWidthIn, (float)uiHeightIn, fFovRadIn,
		fNearZIn, fFarZIn
	),
	ARectangle(
		uiWidthIn,
		uiHeightIn
	)
{
}

ACamera::~ACamera()
{

}

void ACamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (IsSwapChainAccesssed)
	{
		SetRectangle(uiWidthIn, uiHeightIn);

		ADepthStencil::Resize(uiWidthIn, uiHeightIn);
		ARenderTarget::Resize(uiWidthIn, uiHeightIn);
		IFilter::Resize(uiWidthIn, uiHeightIn);
		AViewable::Resize(uiWidthIn, uiHeightIn);

		// 백버퍼 및 Swap Chain 사이즈 변경
		ASwapChainAccessable::cpTexture2D.ReleaseAndGetAddressOf();
		DirectXDevice::pSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		SetAsSwapChainBackBuffer();

		// 뷰포트 변경
		sViewPort.Width = (float)uiWidthIn;
		sViewPort.Height = (float)uiHeightIn;
	}
}

void ACamera::SetAsSwapChainBackBuffer()
{
	ASwapChainAccessable::SetAsSwapChainBackBuffer();

	if (IsDiffWithBackBuffer())
	{
		IFilter::uiArraySize = 1;
		IFilter::uiNumQualityLevels = ASwapChainAccessable::uiNumQualityLevels;
		IFilter::uiBindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		IFilter::uiCPUAccess = NULL;
		IFilter::uiMiscFlag = NULL;
		IFilter::eUsage = D3D11_USAGE_DEFAULT;
		IFilter::eFormat = ASwapChainAccessable::eFormat;

		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, uiWidth, uiHeight,
			IFilter::uiArraySize, ASwapChainAccessable::uiNumQualityLevels, 
			IFilter::uiBindFlag, IFilter::uiCPUAccess, 
			IFilter::uiMiscFlag, IFilter::eUsage, IFilter::eFormat,
			IFilter::cpTexture2D.GetAddressOf()
		);
		ID3D11Helper::CreateUnorderedAccessView(
			DirectXDevice::pDevice,
			IFilter::cpTexture2D.Get(),
			IFilter::cpUAV.GetAddressOf()
		);
	}
}

void ACamera::ReleaseAndGetAddressOfFromSwapChain()
{
	ASwapChainAccessable::ReleaseAndGetAddressOfFromSwapChain();
	IFilter::cpTexture2D.ReleaseAndGetAddressOf();
	IFilter::cpUAV.ReleaseAndGetAddressOf();
}

void ACamera::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	if (IsDiffWithBackBuffer())
	{
		//ARenderTarget의 결과를 IFilter로 녹여준다.
		Shaders& shaders = Shaders::GetInstance();

		ID3D11ComputeShader* pComputeShader = nullptr;

		// Case에 대한 구분 =============================================
		pComputeShader = shaders.GetComputeShader(Shaders::MS16ToSS8ComputeShader);
		// ==============================================================	

		DirectXDevice::pDeviceContext->CSSetShader(pComputeShader, NULL, NULL);
		DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppInputSRV);
		DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, IFilter::cpUAV.GetAddressOf(), nullptr);
		DirectXDevice::pDeviceContext->Dispatch(
			uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
			uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
			uiThreadGroupCntZ);
		SetUAVBarrier();
	}
}

void ACamera::SetUAVBarrier()
{
	ID3D11ShaderResourceView* pReleaseAndGetAddressOfSRV = nullptr;
	ID3D11UnorderedAccessView* pReleaseAndGetAddressOfUAV = nullptr;
	ID3D11SamplerState* pSampler = nullptr;

	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pReleaseAndGetAddressOfSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pReleaseAndGetAddressOfUAV, nullptr);
}